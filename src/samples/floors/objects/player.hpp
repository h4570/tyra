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

#include "../managers/floor_manager.hpp"
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

    void update(Pad &pad, Camera &camera, FloorManager &floorManager);
    void onBeforePlayerFloorMove(Floor *floor, float &newY);

private:
    Vector3 playerNextPosition;
    void getMinMax(Mesh *t_mesh, Vector3 *t_min, Vector3 *t_max);
    void updatePosition(Pad &pad, Camera &camera, FloorManager &floorManager);
    void updateGravity(Pad &pad, FloorManager &floorManager);
};

#endif
