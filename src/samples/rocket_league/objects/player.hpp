/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _PLAYER_
#define _PLAYER_

#include "../camera.hpp"
#include "car.hpp"
#include <modules/pad.hpp>
#include <modules/texture_repository.hpp>
#include <models/mesh.hpp>
#include <tamtypes.h>

/** Player 3D object class  */
class Player : public Car
{

public:
    Player(TextureRepository *t_texRepo);
    ~Player();

    void update(const Pad &t_pad, const Camera &t_camera);

private:
    TextureRepository *texRepo;
};

#endif
