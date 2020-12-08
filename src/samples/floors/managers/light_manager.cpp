/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "light_manager.hpp"
#include <utils/debug.hpp>

// ----
// Constructors/Destructors
// ----

LightManager::LightManager()
{
    PRINT_LOG("Creating light manager!");
    bulbsCount = 2;

    bulbs = new LightBulb[2];
    bulb1Dir = 0;
    bulbs[0].intensity = 1;
    bulbs[0].position.set(30.00F, 30.00F, -30.00F);

    bulb2Dir = 1;
    bulbs[1].intensity = 1;
    bulbs[1].position.set(-30.00F, 30.00F, 30.00F);

    audioOffset = audioTick = 0;

    PRINT_LOG("Light manager created!");
}

LightManager::~LightManager()
{
    delete[] bulbs;
}

// ----
// Methods
// ----

/** Light bulb moving test */
void LightManager::update()
{
    updateBulb1();
    updateBulb2();
}

void LightManager::updateBulb1()
{
    if (bulb1Dir == 1)
    {
        if (areLightsInverted)
        {
            bulbs[0].position.x += 0.6F;
            bulbs[0].position.z -= 0.6F;
        }
        else
        {
            bulbs[0].position.x += 0.6F;
            bulbs[0].position.z += 0.6F;
        }
    }
    else
    {
        if (areLightsInverted)
        {
            bulbs[0].position.x -= 0.6F;
            bulbs[0].position.z += 0.6F;
        }
        else
        {
            bulbs[0].position.x -= 0.6F;
            bulbs[0].position.z -= 0.6F;
        }
    }
}

void LightManager::updateBulb2()
{
    if (bulb2Dir == 1)
    {
        if (areLightsInverted)
        {
            bulbs[1].position.x -= 0.6F;
            bulbs[1].position.z += 0.6F;
        }
        else
        {
            bulbs[1].position.x += 0.6F;
            bulbs[1].position.z += 0.6F;
        }
    }
    else
    {
        if (areLightsInverted)
        {
            bulbs[1].position.x += 0.6F;
            bulbs[1].position.z -= 0.6F;
        }
        else
        {
            bulbs[1].position.x -= 0.6F;
            bulbs[1].position.z -= 0.6F;
        }
    }
}

/** Called by audio thread */
void LightManager::onAudioTick()
{
    bulb1Dir = bulb1Dir == 1 ? 0 : 1;
    bulb2Dir = bulb2Dir == 1 ? 0 : 1;
    if (audioTick++ > 17)
    {
        if (++audioOffset > 3)
        {
            audioOffset = 0;
            if (areLightsInverted)
            {
                areLightsInverted = false;
                bulbs[0].position.x = 20.0F;
                bulbs[0].position.z = -20.0F;

                bulbs[1].position.x = -20.0F;
                bulbs[1].position.z = 20.0F;
            }
            else
            {
                areLightsInverted = true;
                bulbs[0].position.x = 20.0F;
                bulbs[0].position.z = 20.0F;

                bulbs[1].position.x = -20.0F;
                bulbs[1].position.z = -20.0F;
            }
        }
    }
    else
    {
        bulbs[0].intensity += 9;
        bulbs[1].intensity += 9;
    }
}
