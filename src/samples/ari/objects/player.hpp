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
#include <tamtypes.h>

/** Player 3D object class  */
class Player
{

public:
    float gravity, velocity, lift;
    u8 isOnFloor, isCollideFloor, indexOfCurrentFloor;
    Mesh mesh;
    Player();
    ~Player();

private:
    Vector3 playerNextPosition;
};

#endif
