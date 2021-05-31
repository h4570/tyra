/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _SKYBOX_
#define _SKYBOX_

#include "../camera.hpp"
#include <modules/texture_repository.hpp>
#include <models/mesh.hpp>
#include <tamtypes.h>

/** Skybox 3D object class  */
class Skybox
{

public:
    Skybox(TextureRepository *t_texRepo);
    ~Skybox();

    Mesh mesh;

private:
    TextureRepository *texRepo;
};

#endif
