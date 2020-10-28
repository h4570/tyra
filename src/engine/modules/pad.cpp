/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/modules/pad.hpp"

#include "../include/utils/debug.hpp"
#include <kernel.h>
#include <loadfile.h>
#include <string.h>
#include <stdio.h>

// ----
// Constructors/Destructors
// ----

/** Init vars, load modules, opens pad port and initializes pad */
Pad::Pad()
{
    this->oldPad = 0;
    this->loadModules();
    padInit(0);
    this->port = 0; // 0 -> Connector 1, 1 -> Connector 2
    this->slot = 0; // Always zero if not using multitap
    if ((this->ret = padPortOpen(this->port, this->slot, padBuf)) == 0)
    {
        PRINT_ERR("padPortOpen failed!");
        printf("padPortOpen returned: %d\n", this->ret);
        SleepThread();
    }
    if (!this->initPad())
    {
        PRINT_ERR("initPad failed!");
        SleepThread();
    }
}

Pad::~Pad() {}

// ----
// Methods
// ----

/** Load SIO2MAN and PADMAN modules */
void Pad::loadModules()
{
    PRINT_LOG("Loading pad modules");
    this->ret = SifLoadModule("rom0:SIO2MAN", 0, NULL);
    if (this->ret < 0)
    {
        PRINT_ERR("SifLoadModule (SIO2MAN) failed!");
        printf("SifLoadModule returned: %d\n", this->ret);
        SleepThread();
    }
    this->ret = SifLoadModule("rom0:PADMAN", 0, NULL);
    if (this->ret < 0)
    {
        PRINT_ERR("SifLoadModule (PADMAN) failed!");
        printf("SifLoadModule returned: %d\n", this->ret);
        SleepThread();
    }
    PRINT_LOG("Pad modules loaded!");
}

/** Wait when pad will be ready (stable and ready) */
int Pad::waitPadReady()
{
    int state;
    int lastState;
    char stateString[16];
    state = padGetState(this->port, this->slot);
    lastState = -1;
    while ((state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1))
    {
        if (state != lastState)
        {
            padStateInt2String(state, stateString);
            PRINT_LOG("Pad state changed");
            printf("Curent pad(%d,%d) status: %s\n", this->port, this->slot, stateString);
        }
        lastState = state;
        state = padGetState(this->port, this->slot);
    }
    // Were the pad ever 'out of sync'?
    if (lastState != -1)
        PRINT_LOG("Pad is ready!");
    return 0;
}

/** Initializes and checks type of pad */
int Pad::initPad()
{
    PRINT_LOG("Initializing pad");
    this->waitPadReady();
    // How many different modes can this device operate in?
    // i.e. get # entrys in the modetable
    int modes = padInfoMode(this->port, this->slot, PAD_MODETABLE, -1);
    if (modes == 0)
    {
        PRINT_ERR("Connected device is not a dual shock controller!"); // (it has no actuator engines)
        return 1;
    }
    // Verify that the controller has a DUAL SHOCK mode
    int i = 0;
    do
    {
        if (padInfoMode(this->port, this->slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
            break;
        i++;
    } while (i < modes);

    if (i >= modes)
    {
        PRINT_ERR("Connected device is not a dual shock controller!");
        return 1;
    }

    // If ExId != 0x0 => This controller has actuator engines
    // This check should always pass if the Dual Shock test above passed
    this->ret = padInfoMode(this->port, this->slot, PAD_MODECUREXID, 0);
    if (this->ret == 0)
    {
        PRINT_ERR("Connected device is not a dual shock controller!");
        return 1;
    }

    PRINT_LOG("Enabling dual shock functions.");

    // When using MMODE_LOCK, user cant change mode with Select button
    padSetMainMode(this->port, this->slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);

    this->waitPadReady();
    printf("Pad has pressure sensitive buttons? %d\n", padInfoPressMode(this->port, this->slot));
    this->waitPadReady();
    padEnterPressMode(this->port, this->slot); // Set pressure sensitive mode
    this->waitPadReady();
    this->actuators = padInfoAct(this->port, this->slot, -1, 0);
    printf("# of actuators: %d\n", this->actuators);
    if (actuators != 0)
    {
        this->actAlign[0] = 0; // Enable small engine
        this->actAlign[1] = 1; // Enable big engine
        this->actAlign[2] = 0xff;
        this->actAlign[3] = 0xff;
        this->actAlign[4] = 0xff;
        this->actAlign[5] = 0xff;
        this->waitPadReady();
        printf("padSetActAlign: %d\n", padSetActAlign(this->port, this->slot, actAlign));
    }
    else
        printf("Did not find any actuators.\n");
    this->waitPadReady();
    PRINT_LOG("Pad initialized!");
    return 1;
}

/** Updates state of joys/buttons */
void Pad::update()
{
    int x = 0;
    this->ret = padGetState(this->port, this->slot);
    while ((this->ret != PAD_STATE_STABLE) && (this->ret != PAD_STATE_FINDCTP1))
    {
        if (this->ret == PAD_STATE_DISCONN)
            printf("Pad(%d, %d) is disconnected\n", this->port, this->slot);
        this->ret = padGetState(this->port, this->slot);
    }
    if (x == 1)
        printf("Pad: OK!\n");

    this->ret = padRead(this->port, this->slot, &this->buttons); // this->port, this->slot, this->buttons

    if (this->ret != 0)
    {
        this->padData = 0xffff ^ this->buttons.btns;

        this->newPad = this->padData & ~this->oldPad;
        this->oldPad = this->padData;
        this->reset();
        this->rJoyH = this->buttons.rjoy_h;
        this->rJoyV = this->buttons.rjoy_v;
        this->lJoyH = this->buttons.ljoy_h;
        this->lJoyV = this->buttons.ljoy_v;

        if (this->newPad & PAD_CROSS)
            this->isCrossClicked = 1;
        if (this->newPad & PAD_SQUARE)
            this->isSquareClicked = 1;
        if (this->newPad & PAD_TRIANGLE)
            this->isTriangleClicked = 1;
        if (this->newPad & PAD_CIRCLE)
            this->isCircleClicked = 1;

        if (this->buttons.up_p > 0)
            this->isDpadUpPressed = 1;
        if (this->buttons.down_p > 0)
            this->isDpadDownPressed = 1;
        if (this->buttons.left_p)
            this->isDpadLeftPressed = 1;
        if (this->buttons.right_p)
            this->isDpadRightPressed = 1;
    }
}

/** Resets state of joys/buttons */
void Pad::reset()
{
    this->isCrossClicked = 0;
    this->isSquareClicked = 0;
    this->isTriangleClicked = 0;
    this->isCircleClicked = 0;
    this->isDpadUpPressed = 0;
    this->isDpadDownPressed = 0;
    this->isDpadLeftPressed = 0;
    this->isDpadRightPressed = 0;
    this->lJoyH = 0;
    this->lJoyV = 0;
    this->rJoyH = 0;
    this->rJoyV = 0;
}
