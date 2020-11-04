/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _LIGHT_MANAGER_
#define _LIGHT_MANAGER_

#include <models/light_bulb.hpp>
#include <models/audio_listener.hpp>

/** Class which is maintaining floors objects */
class LightManager
{

public:
    LightManager();
    ~LightManager();

    LightBulb *bulbs;
    u8 bulbsCount;
    void update();
    void onAudioTick();

private:
    u8 bulb1Dir, bulb2Dir;
    void updateBulb1();
    void updateBulb2();
    u8 audioOffset, areLightsInverted;
    u32 audioTick;
};

#endif
