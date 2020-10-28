

/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_RENDER_DATA_
#define _TYRA_RENDER_DATA_

#include "light_bulb.hpp"
#include <draw_primitives.h>
#include "math/matrix.hpp"
#include "math/plane.hpp"

struct RenderData
{
    LightBulb *bulbs;
    u16 bulbsCount;
    /** Camera (lookAt) */
    Matrix *worldView;
    /** Perspective projection */
    Matrix *perspective;
    Vector3 *cameraPosition;
    Plane *frustumPlanes;
    prim_t *prim;
};

#endif
