/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_LIGHT_
#define _TYRA_LIGHT_

#include <math3d.h>
#include <tamtypes.h>
#include "../models/math/vector3.hpp"
#include "../models/light_bulb.hpp"

class Light
{

public:
    Light();
    ~Light();

    static u16 getLightsCount(u32 t_bulbsCount);
    static void calculateLight(VECTOR *t_lightDirections, VECTOR *t_lightColors, int *t_lightTypes, LightBulb *t_bulbs, u32 t_lightsCount, Vector3 t_objPosition);

private:
};

#endif
