/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_OBJECT3D_SPEC_
#define _TYRA_OBJECT3D_SPEC_

#include "math/vector3.hpp"
#include "./mesh_texture.hpp"
#include <tamtypes.h>
#include <draw_buffers.h>
#include <draw_sampling.h>

/** Class which contain 3D object specification */
class MeshSpec
{

public:
    clutbuffer_t clut;
    lod_t lod;

    MeshSpec();
    ~MeshSpec();

    void allocateMemory();
    void setupLodAndClut();
    MeshTexture *textures;

private:
};

#endif
