/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#ifndef _PLAYER_
#define _PLAYER_

#define WATER_LEVEL -5.0F

#include "../camera.hpp"
#include <audsrv.h>
#include <modules/pad.hpp>
#include <tamtypes.h>
class Engine;
class Player
{

public:
    float gravity, velocity, lift;
    Mesh mesh;
    Player();
    ~Player();
    void init(Engine *t_engine);
    void update(Pad &t_pad);
    const u8 &isJumping() { return bIsJumping; }
    const u8 &getLifes() { return lifes; }
    void setLife(const u8 &l) { lifes = lifes = l < 4 ? l : 3; }

private:
    u8 bIsJumping;
    Vector3 playerNextPosition;
    u8 lifes;
    audsrv_adpcm_t *waterImpact;
    Engine *pEngine;
    u8 bIsImpactingWater;
};

#endif
