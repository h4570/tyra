/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_LIGHT_BULB_
#define _TYRA_LIGHT_BULB_

#include "math/vector3.hpp"
#include <tamtypes.h>

struct LightBulb
{
    Vector3 position;
    u8 intensity;
};

#endif
