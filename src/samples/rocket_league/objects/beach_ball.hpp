/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _BEACH_BALL_
#define _BEACH_BALL_

#include "../camera.hpp"
#include <modules/texture_repository.hpp>
#include <models/math/vector3.hpp>
#include <models/mesh.hpp>
#include <tamtypes.h>

/** BeachBall 3D object class  */
class BeachBall
{

public:
    BeachBall(TextureRepository *t_texRepo);
    ~BeachBall();

    Mesh mesh;

    void push(const Vector3 &car);
    void update();

private:
    float gravity, velocity, lift;
    Vector3 force;
    void updateGravity();
    TextureRepository *texRepo;
};

#endif
