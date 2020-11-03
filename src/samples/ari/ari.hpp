/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _ARI_
#define _ARI_

#include <tamtypes.h>
#include <game.hpp>
#include <engine.hpp>
#include "objects/player.hpp"
#include <models/light_bulb.hpp>
#include <modules/texture_repository.hpp>
#include "./camera.hpp"

class Ari : public Game
{

public:
    Ari(const Engine &t_engine);
    ~Ari();

    void onInit();
    void onUpdate();

    Engine engine;

private:
    void initBulb();
    void calcSpiral(int X, int Y);
    LightBulb bulb;
    Player player;
    Mesh island, islandAddons, skybox;
    Mesh *waterFloors;
    Point *spirals;
    Camera camera;
    TextureRepository *texRepo;
};

#endif
