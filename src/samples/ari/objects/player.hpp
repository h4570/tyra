/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _PLAYER_
#define _PLAYER_

#include "../camera.hpp"
#include <modules/pad.hpp>
#include <models/mesh_spec.hpp>
#include <tamtypes.h>

/** Player 3D object class  */
class Player
{

public:
    float gravity, velocity, lift;
    u8 isOnFloor, isCollideFloor, indexOfCurrentFloor;
    Mesh mesh;
    MeshSpec *spec;
    Player();
    ~Player();

private:
    Vector3 playerNextPosition;
};

#endif
