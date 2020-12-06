/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _UI_
#define _UI_

#include <modules/texture_repository.hpp>
#include <modules/renderer.hpp>
#include "objects/player.hpp"
#include <tamtypes.h>

class Ui
{

public:
    Ui(TextureRepository *t_texRepo);
    ~Ui();

    void update(const Player &player);
    void render(Renderer *t_renderer);

private:
    u8 isTask1Done, isTask2Done;
    Sprite task1, task2, task1Checkbox, task2Checkbox;
    Texture *grayCheckboxTex, *blueCheckboxTex;
};

#endif
