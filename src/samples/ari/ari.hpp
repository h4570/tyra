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
#include "./camera.hpp"

class Ari : public Game
{

public:
    Ari();
    ~Ari();

    void onInit();
    void onUpdate();

    Engine engine;

private:
    void initBulb();
    LightBulb bulb;
    Player *player;
    Camera *camera;
};

#endif
