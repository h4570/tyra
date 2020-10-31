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
#include "../models/mesh_frame.hpp"
#include "../loaders/bmp_loader.hpp"

/** Class responsible for intializing draw env, textures and buffers */
class TextureRepository
{

public:
    TextureRepository();
    ~TextureRepository();

    // ----
    // Getters
    // ----

    /** Returns all repository textures. */
    MeshTexture **getAll() const { return textures; }

    const u32 &getTexturesCount() const { return texturesCount; };

    /** 
     * Returns single texture.
     * NULL if not found.
     */
    MeshTexture *getByMesh(const u32 &t_meshId, const u32 &t_materialId)
    {
        for (u32 i = 0; i < texturesCount; i++)
            if (textures[i]->isLinkedWith(t_meshId, t_materialId))
                return textures[i];
        return NULL;
    }

    /** 
     * Returns single texture.
     * NULL if not found.
     */
    MeshTexture *getByTextureId(const u32 &t_id) const
    {
        for (u32 i = 0; i < texturesCount; i++)
            if (t_id == textures[i]->getId())
                return textures[i];
        return NULL;
    }

    // ----
    //  Setters
    // ----

    // ----
    //  Other
    // ----

    /** Add single unlinked texture to repository. */
    MeshTexture *add(char *t_subfolder, char *t_name);

    /** Add linked textures in given subpath for mesh material names. */
    void addByMesh(char *t_path, Mesh &mesh);

    /** 
     * Remove texture from repository.
     * Texture is NOT destructed.
     */
    void remove(u32 textureId);

private:
    void increaseArray(u32 newLength);
    u32 texturesCount;
    MeshTexture **textures;
    BmpLoader loader;
};

#endif
