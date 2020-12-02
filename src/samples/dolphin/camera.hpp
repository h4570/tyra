/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#ifndef _CAMERA_
#define _CAMERA_

#include <modules/pad.hpp>
#include <models/mesh.hpp>
#include <models/math/vector3.hpp>
#include <models/screen_settings.hpp>
#include <modules/camera_base.hpp>
#include <tamtypes.h>

class Camera : public CameraBase
{

public:
    Vector3 position,up, unitCirclePosition;
    float horizontalLevel, verticalLevel;

    Camera(ScreenSettings *t_screen);
    ~Camera();

    void update(Pad &t_pad, Mesh &t_mesh);
    void rotate(Pad &t_pad);
    void followBy(Mesh &t_mesh);

protected:
    Vector3 *getPosition() { return &position; };
    Vector3 *getUp() { return &up; };
    ScreenSettings screen;
};

#endif
