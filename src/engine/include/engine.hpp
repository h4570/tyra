/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_ENGINE_
#define _TYRA_ENGINE_

#include <tamtypes.h>
#include "game.hpp"
#include "models/screen_settings.hpp"
#include "models/math/matrix.hpp"
#include "modules/renderer.hpp"
#include "modules/timer.hpp"
#include "modules/pad.hpp"
#include "modules/audio.hpp"

class Engine
{

public:
    Engine();
    ~Engine();

    void init(Game *t_game, u32 t_gifPacketSize);
    void setDefaultScreen();
    void setScreen(ScreenSettings &t_settings);
    Renderer *renderer;
    Audio audio;
    ScreenSettings screen;
    Pad pad;
    float fps;

private:
    u8 fpsDelayer;
    Timer timer;
    u8 isInitialized, isScreenInitialized;
    void gameLoop();
    Game *game;
    s32 mainThreadId;
    static void wakeup(s32 t_alarmId, u16 t_time, void *t_common);

    // ThreadManager threadManager;
};

#endif