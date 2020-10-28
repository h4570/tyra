/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/modules/light.hpp"

// ----
// Constructors/Destructors
// ----

Light::Light() {}

Light::~Light() {}

// ----
// Methods
// ----

const u8 ADDITIONAL_LIGHTS = 1; // Ambient

u16 Light::getLightsCount(u32 t_bulbsCount) { return t_bulbsCount + ADDITIONAL_LIGHTS; }

/** Calculates lighting. Used in gifSender in object 3D calculations */
void Light::calculateLight(VECTOR *t_lightDirections, VECTOR *t_lightColors, int *t_lightTypes, LightBulb *t_bulbs, u32 t_bulbsCount, Vector3 t_objPosition)
{
    // --- Ambient light

    t_lightTypes[0] = LIGHT_AMBIENT;

    t_lightDirections[0][0] = 0.0F;
    t_lightDirections[0][1] = 0.0F;
    t_lightDirections[0][2] = 0.0F;
    t_lightDirections[0][3] = 1.0F;

    t_lightColors[0][0] = 0.0F;
    t_lightColors[0][1] = 0.0F;
    t_lightColors[0][2] = 0.0F;
    t_lightColors[0][3] = 1.0F;

    // ---

    for (u8 i = 0; i < t_bulbsCount; i++)
    {
        t_lightTypes[i + 1] = LIGHT_DIRECTIONAL;

        Vector3 newLight = Vector3(t_objPosition.x - t_bulbs[i].position.x,
                                   t_objPosition.y - t_bulbs[i].position.y,
                                   t_objPosition.z - t_bulbs[i].position.z);

        newLight.normalize();
        newLight = newLight *
                   (.1F +
                    (t_bulbs[i].intensity / t_bulbs[i].position.distanceTo(t_objPosition)));

        t_lightDirections[i + 1][0] = newLight.x;
        t_lightDirections[i + 1][1] = newLight.y;
        t_lightDirections[i + 1][2] = newLight.z;
        t_lightDirections[i + 1][3] = 1.0F;

        t_lightColors[i + 1][0] = 0.8F;
        t_lightColors[i + 1][1] = 0.8F;
        t_lightColors[i + 1][2] = 0.8F;
        t_lightColors[i + 1][3] = 1.0F;
    }
}
