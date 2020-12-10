/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#ifndef _DRIVER_
#define _DRIVER_

#include <tamtypes.h>
#include <audsrv.h>
#include <game.hpp>
#include <engine.hpp>
#include <models/light_bulb.hpp>
#include <models/sprite.hpp>
#include <modules/texture_repository.hpp>
#include "./camera.hpp"
#include "./objects/player.hpp"
#include "./objects/collectible.hpp"
#include "./objects/mine.hpp"

class Dolphin : public Game
{

public:
    Dolphin(Engine *t_engine);
    ~Dolphin();

    void onInit();
    void onUpdate();

    Engine *engine;
    static float engineFPS;

private:
    /*DO NOT TOUCH ORDER OF DECLARATION!
    GCC BUG CAUSES RENDERING TO FAIL IN ANY OTHER ORDER OF DECLARATION!*/
    Mesh seabed;
    Mesh island;
    Mesh skybox;
    Mesh mine;
    LightBulb bulb;
    Player player;
    Camera camera;
    Collectible *oysters;
    TextureRepository *texRepo;
    Sprite waterOverlay;
    Mesh water;
    Mesh waterbox;
    Sprite lifeSprites[3];
    Sprite gameOver;
    audsrv_adpcm_t *underwaterAmbient, *surfaceAmbient;
    audsrv_adpcm_t *pickupSound;
    audsrv_adpcm_t *boomSound;
    Mine *mines;
};

#endif
