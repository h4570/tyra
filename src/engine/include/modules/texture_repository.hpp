/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_TEXTURE_REPOSITORY_
#define _TYRA_TEXTURE_REPOSITORY_

#include <tamtypes.h>
#include <draw_buffers.h>
#include "../models/mesh.hpp"

/** Class responsible for intializing draw env, textures and buffers */
class TextureRepository
{

public:
    TextureRepository();
    ~TextureRepository();

private:
    void allocateTextureBuffer(u16 t_width, u16 t_height);
    void deallocateTextureBuffer();
    void changeTexture(Mesh *t_mesh, u8 t_textureIndex);
    texbuffer_t textureBuffer;
};

#endif
