/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "include/engine.hpp"

#include <kernel.h>
#include <stdio.h>
#include <sifrpc.h>
#include <time.h>
#include <cstdlib>

#include "include/modules/vu1.hpp"
#include "include/utils/debug.hpp"

// VU1 micro program
extern u32 VU1Draw3D_CodeStart __attribute__((section(".vudata")));
extern u32 VU1Draw3D_CodeEnd __attribute__((section(".vudata")));
//

// ----
// Constructors/Destructors
// ----

Engine::Engine()
{
    SifInitRpc(0);
    srand(time(NULL));
    VU1::uploadProgram(0, &VU1Draw3D_CodeStart, &VU1Draw3D_CodeEnd);
    audio.startThread();
    isInitialized = 0;
    isScreenInitialized = 0;
    mainThreadId = GetThreadId();
}

Engine::~Engine() {}

// ----
// Methods
// ----

void Engine::setScreen(ScreenSettings &t_settings)
{
    screen = t_settings;
    isScreenInitialized = true;
}

void Engine::setDefaultScreen()
{
    screen.projectionScale = 4096.0F;
    screen.nearPlaneDist = 2.0F;
    screen.farPlaneDist = 2000.0F;
    screen.fov = 60.0F;
    screen.aspectRatio = 4.0F / 3.0F;
    screen.width = 640.0F;
    screen.height = 480.0F;
    isScreenInitialized = true;
}

void Engine::init(Game *t_game, u32 t_gifPacketSize)
{
    if (!isScreenInitialized)
        PRINT_ERR("Cant init because screen was not set!");
    else if (isInitialized)
        PRINT_ERR("Already initialized!");
    else
    {
        game = t_game;
        renderer = new Renderer(t_gifPacketSize, &screen);
        isInitialized = true;
        game->onInit();
        gameLoop();
    }
}

/** Do not call this method. This is used in gameLoop() to maintain multithreading */
void Engine::wakeup(s32 t_alarmId, u16 t_time, void *t_common)
{
    (void)t_alarmId;
    (void)t_time;
    iWakeupThread(*(int *)t_common);
    ExitHandler();
}

void Engine::gameLoop()
{
    for (;;)
    {
        pad.update();
        game->onUpdate();
        if (fpsDelayer++ >= 4)
        {
            fps = timer.getFPS();
            fpsDelayer = 0;
        }
        timer.primeTimer();
        renderer->endFrame(fps);
        /** -6~ FPS */
        SetAlarm(fps > 49.0F ? 24 : 48, &Engine::wakeup, &mainThreadId);
        SleepThread();
    }
}
