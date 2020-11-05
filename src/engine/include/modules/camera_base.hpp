/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_CAMERA_BASE_
#define _TYRA_CAMERA_BASE_

#include "../models/math/plane.hpp"
#include "../models/math/matrix.hpp"
#include "../models/math/vector3.hpp"
#include "../models/screen_settings.hpp"

/** Class responsible for frustum culling */
class CameraBase
{

public:
    CameraBase(ScreenSettings *t_screen, Vector3 *t_position, Vector3 *t_up);
    virtual ~CameraBase(){};

    void updatePlanes(Vector3 t_target);
    Plane planes[6];
    Matrix worldView;

protected:
    ScreenSettings *screen;

private:
    Vector3 *p_position, *p_up;
    float farPlaneDist, nearPlaneDist, nearHeight, nearWidth, farHeight, farWidth;
    Vector3 ftl, ftr, fbl, fbr, ntl, ntr, nbl, nbr;
};

#endif
