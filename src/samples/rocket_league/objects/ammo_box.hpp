/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _AMMO_BOX_
#define _AMMO_BOX_

#include "../camera.hpp"
#include <modules/texture_repository.hpp>
#include <models/mesh.hpp>
#include <tamtypes.h>

/** AmmoBox 3D object class  */
class AmmoBox
{

public:
    AmmoBox(TextureRepository *t_texRepo);
    ~AmmoBox();

    Mesh mesh;

private:
    TextureRepository *texRepo;
};

#endif
