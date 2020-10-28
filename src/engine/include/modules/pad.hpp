/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_PAD_
#define _TYRA_PAD_

#include <tamtypes.h>
#include <libpad.h>

/** Class responsible for player pad */
class Pad
{

public:
    u8 isCrossClicked, isSquareClicked, isTriangleClicked, isCircleClicked;
    u8 isDpadUpPressed, isDpadDownPressed, isDpadLeftPressed, isDpadRightPressed;
    u8 lJoyH, lJoyV, rJoyH, rJoyV;

    Pad();
    ~Pad();
    void update();

private:
    char padBuf[256] __attribute__((aligned(64)));
    char actAlign[6];
    int actuators, ret, port, slot;
    padButtonStatus buttons;
    u32 padData, oldPad, newPad;

    void reset();
    void loadModules();
    int waitPadReady();
    int initPad();
};

#endif
