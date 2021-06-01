/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _POLICE_CAR_
#define _POLICE_CAR_

#include "../camera.hpp"
#include <modules/texture_repository.hpp>
#include <models/mesh.hpp>
#include <tamtypes.h>

/** PoliceCar 3D object class  */
class PoliceCar
{

public:
    PoliceCar(TextureRepository *t_texRepo);
    ~PoliceCar();

    Mesh mesh;

private:
    TextureRepository *texRepo;
};

#endif
