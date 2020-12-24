/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/modules/camera_base.hpp"

#include <fastmath.h>
#include "../include/utils/debug.hpp"
#include "../include/utils/math.hpp"

// ----
// Constructors/Destructors
// ----

CameraBase::CameraBase(ScreenSettings *t_screen, Vector3 *t_position)
    : screen(t_screen)
{
    PRINT_LOG("Initializing frustum");
    farPlaneDist = screen->farPlaneDist;
    nearPlaneDist = screen->nearPlaneDist;
    float tang = tanf(screen->fov * Math::HALF_ANG2RAD);
    nearHeight = tang * nearPlaneDist;
    nearWidth = nearHeight * screen->aspectRatio;
    farHeight = tang * farPlaneDist;
    farWidth = farHeight * screen->aspectRatio;
    p_position = t_position;
    up.set(0.0F, 1.0F, 0.0F);
    PRINT_LOG("CameraBase initialized!");
}

// ----
// Methods
// ----

void CameraBase::lookAt(Vector3 &t_target)
{
    updatePlanes(t_target);
    worldView.lookAt(*p_position, t_target);
}

void CameraBase::updatePlanes(Vector3 t_target)
{
    Vector3 nearCenter, farCenter, X, Y, Z;
    // compute the Z axis of camera
    Z = *p_position - t_target;
    Z.normalize();
    // X axis of camera of given "up" vector and Z axis
    X = up * Z;
    X.normalize();
    // the real "up" vector is the cross product of Z and X
    Y = Z * X;
    // compute the center of the near and far planes
    nearCenter = *p_position - Z * nearPlaneDist;
    farCenter = *p_position - Z * farPlaneDist;
    // compute the 8 corners of the frustum
    ntl = nearCenter + Y * nearHeight - X * nearWidth;
    ntr = nearCenter + Y * nearHeight + X * nearWidth;
    nbl = nearCenter - Y * nearHeight - X * nearWidth;
    nbr = nearCenter - Y * nearHeight + X * nearWidth;

    ftl = farCenter + Y * farHeight - X * farWidth;
    fbr = farCenter - Y * farHeight + X * farWidth;
    ftr = farCenter + Y * farHeight + X * farWidth;
    fbl = farCenter - Y * farHeight - X * farWidth;

    planes[0].update(ntr, ntl, ftl); // Top
    planes[1].update(nbl, nbr, fbr); // BOTTOM
    planes[2].update(ntl, nbl, fbl); // LEFT
    planes[3].update(nbr, ntr, fbr); // RIGHT
    planes[4].update(ntl, ntr, nbr); // NEAR
    planes[5].update(ftr, ftl, fbl); // FAR
}
