/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_MESH_TEXTURE_
#define _TYRA_MESH_TEXTURE_

#include <tamtypes.h>
#include <draw_sampling.h>

/** 
 * Class which contains texture data
 * Textures are paired with material via material id.
 * Additionaly, you can load two textures for single material
 * and switch materialId here to change texture.
 */
class MeshTexture
{

public:
    MeshTexture();
    ~MeshTexture();

    // ----
    // Getters
    // ----

    // ----
    //  Setters
    // ----

    // ----
    //  Other
    // ----

    // TODO

    u32 id;
    unsigned char *data;
    texwrap_t wrapSettings;
    char *name;
    u8 width, height;

private:
};

#endif
