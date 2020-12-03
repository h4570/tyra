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

class Dolphin : public Game
{

public:
    Dolphin(Engine *t_engine);
    ~Dolphin();

    void onInit();
    void onUpdate();

    Engine *engine;

private:
    LightBulb bulb;
    Player player;
    Camera camera;
    Mesh island, skybox, water, waterbox;
    TextureRepository *texRepo;
    Sprite waterOverlay;
};

#endif
