/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _ENEMY_
#define _ENEMY_

#include <tamtypes.h>
#include "../managers/floor_manager.hpp"
#include <modules/texture_repository.hpp>

/** Player 3D object class  */
class Enemy
{

public:
    s16 indexOfCurrentFloor;
    Enemy(TextureRepository *t_texRepo);
    ~Enemy();

    inline Mesh *getMeshes() const { return meshes; }
    inline u8 getMeshesCount() const { return 3; }
    inline Vector3 getPosition() const { return position; }
    void setPosition(const Vector3 &t_vec);
    void setPosition(const float &t_y);
    void kill(const Player &player);
    void update(const FloorManager &t_floorManager);

private:
    /** 0 - head, 1 - body, 2 - weapon */
    Mesh *meshes;
    const Floor *currentFloor;
    Vector3 position, currFloorMin, currFloorMax;
    u8 isKilled;
    void updateGravity();
};

#endif
