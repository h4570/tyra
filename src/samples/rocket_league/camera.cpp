/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "camera.hpp"

#include <utils/debug.hpp>
#include <fastmath.h>

const float CAMERA_Y = 60.0F;

// ----
// Constructors/Destructors
// ----

Camera::Camera(ScreenSettings *t_screen) : CameraBase(t_screen, &position)
{
    consoleLog("Initializing camera");
    verticalLevel = 120.0F;
    consoleLog("Camera initialized!");
}

Camera::~Camera() {}

// ----
// Methods
// ----

void Camera::update(Pad &t_pad, Mesh &t_mesh)
{
    rotate(t_pad);
    followBy(t_mesh);
    lookAt(t_mesh.position);
}

/** Set camera rotation by pad right joy and update unit circle
 * https://en.wikipedia.org/wiki/Unit_circle
 * https://www.desmos.com/calculator/3e7iypw4ow
 */
void Camera::rotate(Pad &t_pad)
{
    if (t_pad.rJoyH <= 100)
        horizontalLevel -= 0.08;
    else if (t_pad.rJoyH >= 200)
        horizontalLevel += 0.08;
    if (t_pad.rJoyV <= 50 && verticalLevel > 25.0F)
        verticalLevel -= 1.8F;
    else if (t_pad.rJoyV >= 200 && verticalLevel < 180.0F)
        verticalLevel += 1.8F;
    unitCirclePosition.x = (sin(horizontalLevel) * verticalLevel);
    unitCirclePosition.y = CAMERA_Y;
    unitCirclePosition.z = (cos(horizontalLevel) * verticalLevel);
}

/** Rotate camera around 3D object */
void Camera::followBy(Mesh &t_mesh)
{
    position.x = unitCirclePosition.x + t_mesh.position.x;
    position.y = unitCirclePosition.y + t_mesh.position.y;
    position.z = unitCirclePosition.z + t_mesh.position.z;
}
