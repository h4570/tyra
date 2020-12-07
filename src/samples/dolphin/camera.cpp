/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#include "camera.hpp"

#include <utils/math.hpp>

const float CAMERA_Y = 15.0F;

Camera::Camera(ScreenSettings *t_screen) : CameraBase(t_screen, &position, &up)
{
    verticalLevel = 30.0F;
    up.x = 0.0F;
    up.y = 1.0F;
    up.z = 0.0F;
}

Camera::~Camera() {}

void Camera::update(Pad &t_pad, Mesh &t_mesh)
{
    float cameraRot = (t_mesh.rotation.z * 180 / Math::PI) - (horizontalLevel * 180 / Math::PI);
    cameraRot += 180;
    cameraRot /= 15;
    horizontalLevel += cameraRot / (180 / Math::PI);
    rotate(t_pad);
    followBy(t_mesh);
    Vector3 lookPos = Vector3(t_mesh.position.x, t_mesh.position.y + 10.0F, t_mesh.position.z);
    lookAt(lookPos);
}

void Camera::rotate(Pad &t_pad)
{

    if (t_pad.rJoyH <= 100)
        horizontalLevel -= 0.08;
    else if (t_pad.rJoyH >= 200)
        horizontalLevel += 0.08;
    /*
    if (t_pad.rJoyV <= 50 && verticalLevel > 25.0F)
        verticalLevel -= 0.9F;
    else if (t_pad.rJoyV >= 200 && verticalLevel < 80.0F)
        verticalLevel += 0.9F;
        */
    unitCirclePosition.x = (Math::sin(horizontalLevel) * verticalLevel);
    unitCirclePosition.y = CAMERA_Y;
    unitCirclePosition.z = (Math::cos(horizontalLevel) * verticalLevel);
}

void Camera::followBy(Mesh &t_mesh)
{
    position.x = unitCirclePosition.x + t_mesh.position.x;
    position.y = unitCirclePosition.y + t_mesh.position.y;
    position.z = unitCirclePosition.z + t_mesh.position.z;

    if (position.x > (-0.04F * verticalLevel) &&
        position.x < 0.0F &&
        position.z > (0.998F * verticalLevel))
    {
        // TODO
        // position.x = 0.0F;
        // position.y = 0.0F;
        // position.z = 0.0F;
        // horizontalLevel = 0.0F;
    }
}