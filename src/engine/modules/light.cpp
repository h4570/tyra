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
const float LIGHT_MAX = 255.0F;
const float LIGHT_DIS_MOD = 4.0F;

void Light::setAmbientLight(const Vector3 &t_rgb) { ambientLight.set(t_rgb); }

u16 Light::getLightsCount(u32 t_bulbsCount) { return t_bulbsCount + ADDITIONAL_LIGHTS; }

/** Calculates lighting. Used in gifSender in object 3D calculations */
void Light::calculateLight(VECTOR *t_lightDirections, VECTOR *t_lightColors, int *t_lightTypes, LightBulb *t_bulbs, u32 t_lightsCount, Vector3 t_objPosition)
{
    // --- Ambient light

    t_lightTypes[0] = LIGHT_AMBIENT;

    t_lightDirections[0][0] = 0.0F;
    t_lightDirections[0][1] = 0.0F;
    t_lightDirections[0][2] = 0.0F;
    t_lightDirections[0][3] = 1.0F;

    t_lightColors[0][0] = ambientLight.x;
    t_lightColors[0][1] = ambientLight.y;
    t_lightColors[0][2] = ambientLight.z;
    t_lightColors[0][3] = 1.0F;

    // ---

    for (u8 i = 1; i < t_lightsCount; i++)
    {
        t_lightTypes[i] = LIGHT_DIRECTIONAL;

        Vector3 newLight = Vector3(t_objPosition.x - t_bulbs[i - 1].position.x,
                                   t_objPosition.y - t_bulbs[i - 1].position.y,
                                   t_objPosition.z - t_bulbs[i - 1].position.z);
        newLight.normalize();
        t_lightDirections[i][0] = newLight.x;
        t_lightDirections[i][1] = newLight.y;
        t_lightDirections[i][2] = newLight.z;
        t_lightDirections[i][3] = 1.0F;

        // Intensity range 0.0F - 1.0F
        float intensity = t_bulbs[i - 1].intensity / LIGHT_MAX;
        // Distance range 0.0F - LIGHT_MAX * LIGHT_DIS_MOD (1020.0F at this time)
        float distance = t_bulbs[i - 1].position.distanceTo(t_objPosition);
        // printf("Distance:%f\n", distance);
        distance = distance > (LIGHT_MAX - LIGHT_DIS_MOD) * LIGHT_DIS_MOD ? LIGHT_MAX - LIGHT_DIS_MOD : distance;
        intensity /= 1.0F + (distance / LIGHT_DIS_MOD); // intensity /= 1.0F - 251.0F
        intensity /= 3.0F;                              // it looks better
        t_lightColors[i][0] = intensity;
        t_lightColors[i][1] = intensity;
        t_lightColors[i][2] = intensity;
        t_lightColors[i][3] = 1.0F;
    }
}
