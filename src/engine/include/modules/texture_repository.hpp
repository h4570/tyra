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

#include <vector>
#include <tamtypes.h>
#include <draw_buffers.h>
#include "../models/mesh.hpp"
#include "../models/mesh_frame.hpp"
#include "../loaders/bmp_loader.hpp"
#include "../loaders/png_loader.hpp"

enum TextureFormat
{
    BMP,
    PNG
};

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
    std::vector<Texture *> *getAll() { return &textures; }

    u32 getTexturesCount() const { return static_cast<u32>(textures.size()); };

    /** 
     * Returns single texture.
     * NULL if not found.
     */
    Texture *getBySprite(const u32 &t_spriteId)
    {
        for (u32 i = 0; i < textures.size(); i++)
            if (textures[i]->isLinkedWith(t_spriteId))
                return textures[i];
        return NULL;
    }

    /** 
     * Returns single texture.
     * NULL if not found.
     */
    Texture *getByMesh(const u32 &t_meshId, const u32 &t_materialId)
    {
        for (u32 i = 0; i < textures.size(); i++)
            if (textures[i]->isLinkedWith(t_meshId, t_materialId))
                return textures[i];
        return NULL;
    }

    /** 
     * Returns single texture.
     * NULL if not found.
     */
    Texture *getByTextureId(const u32 &t_id) const
    {
        for (u32 i = 0; i < textures.size(); i++)
            if (t_id == textures[i]->getId())
                return textures[i];
        return NULL;
    }

    /** 
     * Returns index of link.
     * -1 if not found.
     */
    const s32 getIndexOf(const u32 &t_texId) const
    {
        for (u32 i = 0; i < textures.size(); i++)
            if (textures[i]->getId() == t_texId)
                return i;
        return -1;
    };

    // ----
    //  Setters
    // ----

    // ----
    //  Other
    // ----

    /** 
     * Add unlinked texture.
     * @param t_subfolder Relative path. Ex.: "textures/"
     * @param t_name Filename without extension. Ex.: "water"
     * @param t_format if you want to use BMP, be sure that you have 
     * BMP with RGB 888 24bit, without color information.
     */
    Texture *add(char *t_subfolder, char *t_name, TextureFormat t_format);

    /** 
     * Add linked textures in given subpath for mesh material names.
     * @param t_path Relative path where textures should be searched.
     * @param t_format if you want to use BMP, be sure that you have 
     * BMP with RGB 888 24bit, without color information.
     */
    void addByMesh(char *t_path, Mesh &mesh, TextureFormat t_format);

    /** 
     * Remove texture from repository.
     * Texture is NOT destructed.
     */
    void removeByIndex(const u32 &t_index) { textures.erase(textures.begin() + t_index); }

    /** 
     * Remove texture from repository.
     * Texture is NOT destructed.
     */
    const void removeById(const u32 &t_texId)
    {
        s32 index = getIndexOf(t_texId);
        if (index != -1)
            removeByIndex(index);
        else
            PRINT_ERR("Cant remove texture, because it was not found!");
    }

private:
    std::vector<Texture *> textures;
    BmpLoader bmpLoader;
    PngLoader pngLoader;
};

#endif
