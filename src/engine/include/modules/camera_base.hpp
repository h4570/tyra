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
    CameraBase(ScreenSettings *t_screen, Vector3 *t_position);
    virtual ~CameraBase(){};

    /**
     * Frustum planes.
     * Set by lookAt() method.
     */
    Plane planes[6];

    /** 
     * Projection matrix
     * Set by lookAt() method.
     */
    Matrix worldView;

protected:
    /** Pointer to screen settings */
    ScreenSettings *screen;

    /**
     * Adaption of OpenGL lookAt camera.
     * Calculates worldView matrix and update frustum planes
     * Should be called every frame.
     */
    void lookAt(Vector3 &t_target);

    /** 
     * Do not call this method unless you know what you do. 
     * Called via lookAt(). 
     * Calculate and update frustum planes.
     * http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
     */
    void updatePlanes(Vector3 t_target);

private:
    Vector3 *p_position;
    float farPlaneDist, nearPlaneDist, nearHeight, nearWidth, farHeight, farWidth;
    Vector3 ftl, ftr, fbl, fbr, ntl, ntr, nbl, nbr;
    Vector3 up; // always 0.0F, 1.0F, 0.0F
};

#endif
