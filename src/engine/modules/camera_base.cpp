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

/** Initializes vars and calculate width/height of near and far plane 
 * @param fov (FOV in radians)/2
 * @param ratio Aspect ratio
*/
CameraBase::CameraBase(ScreenSettings *t_screen, Vector3 *t_position, Vector3 *t_up, Vector3 *t_unitCirclePosition)
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
    position2 = t_position;
    up2 = t_up;
    unitCirclePosition2 = t_unitCirclePosition;
    PRINT_LOG("CameraBase initialized!");
}

// ----
// Methods
// ----

/** Calculates and updates frustum planes
 * http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
 */
void CameraBase::updatePlanes(Vector3 t_target)
{
    Vector3 nearCenter, farCenter, X, Y, Z;
    // compute the Z axis of camera
    Z = *position2 - t_target;
    Z.normalize();
    // X axis of camera of given "up" vector and Z axis
    X = *up2 * Z;
    X.normalize();
    // the real "up" vector is the cross product of Z and X
    Y = Z * X;
    // compute the center of the near and far planes
    nearCenter = *position2 - Z * nearPlaneDist;
    farCenter = *position2 - Z * farPlaneDist;
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
