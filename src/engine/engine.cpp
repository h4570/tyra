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
#include "include/utils/debug.hpp"

// ----
// Constructors/Destructors
// ----

Engine::Engine()
{
    setDefaultScreen();
    firePS2();
}

Engine::Engine(const ScreenSettings &t_screen)
{
    screen = t_screen;
    firePS2();
}

Engine::~Engine() {}

// ----
// Methods
// ----

void Engine::setDefaultScreen()
{
    screen.projectionScale = 4096.0F;
    screen.nearPlaneDist = 2.0F;
    screen.farPlaneDist = 2000.0F;
    screen.fov = 60.0F;
    screen.aspectRatio = 4.0F / 3.0F;
    screen.width = 640.0F;
    screen.height = 480.0F;
}

void Engine::init(Game *t_game, u32 t_gifPacketSize)
{
    if (isInitialized)
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

void Engine::firePS2()
{
    SifInitRpc(0);
    srand(time(NULL));
    fileService.startThread();
    audio.startThread(&fileService);
    isInitialized = 0;
    mainThreadId = GetThreadId();
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
        timer.prime();
        renderer->endFrame(fps);
        /** -6~ FPS */
        SetAlarm(150, &Engine::wakeup, &mainThreadId);
        SleepThread();
    }
}
