/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _RACE_TRACK_
#define _RACE_TRACK_

#include "../camera.hpp"
#include <modules/texture_repository.hpp>
#include <models/mesh.hpp>
#include <tamtypes.h>

/** RaceTrack 3D object class  */
class RaceTrack
{

public:
    RaceTrack(TextureRepository *t_texRepo);
    ~RaceTrack();

    Mesh mesh;

private:
    TextureRepository *texRepo;
};

#endif
