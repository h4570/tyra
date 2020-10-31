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
#include "./texture_wrap_settings.hpp"
#include "./texture_link.hpp"
#include "../include/utils/debug.hpp"
#include <vector>

/** 
 * Class which contains texture data.
 * Textures are paired with meshes via addUsage(),
 * removeUsage() functions which use meshId and materialId.
 */
class MeshTexture
{

public:
    MeshTexture();
    ~MeshTexture();

    // ----
    // Getters
    // ----

    /** 
     * Auto generated unique Id. 
     * Core role of this variable is to dont send 
     * again texture in renderer, when last sent texture id
     * by renderer will be equal to this id.
     */
    const u32 &getId() const { return id; };

    const u8 &getWidth() const { return width; };

    const u8 &getHeight() const { return height; };

    u32 getTextureLinksCount() const { return static_cast<u32>(texLinks.size()); };

    texwrap_t *getWrapSettings() { return &wrapSettings; };

    /** 
     * Returns always width * width * 3.
     * 3 because of RGB
     */
    u32 getDataSize() const { return width * height * 3; };

    /** 
     * Texture data, used by renderer.
     * Array of size getDataSize().
     */
    unsigned char *getData() const { return data; };

    /** 
     * Get texture name.
     * For "textures/abc.bmp" result will be: "abc"
     */
    char *getName() const { return name; };

    /** Array of texture links. Size of getTextureLinksCount() */
    const std::vector<TextureLink> &getTextureLinks() const { return texLinks; };

    /** 
     * Returns index of link.
     * -1 if not found.
     */
    const s32 getIndexOfLink(const u32 &t_meshId, const u32 &t_materialId) const
    {
        for (u32 i = 0; i < texLinks.size(); i++)
            if (texLinks[i].materialId == t_materialId && texLinks[i].meshId == t_meshId)
                return i;
        return -1;
    };

    // ----
    //  Setters
    // ----

    /** 
     * Set texture size and allocate texture data
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
     */
    void setSize(const u8 &t_width, const u8 &t_height);

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
     */
    void setHeight(const u8 &t_val) { height = t_val; }

    /** 
     * Set texture name. 
     * Should be the file name without extension
     */
    void setData(const u32 &t_index, const unsigned char &t_val) { data[t_index] = t_val; }

    /** 
     * Set texture name. 
     * Should be the file name without extension
     */
    void setName(char *t_val);

    /** Set texture wrapping */
    void setWrapSettings(const WrapSettings t_horizontal, const WrapSettings t_vertical);

    // ----
    //  Other
    // ----

    /** Assign texture to mesh and mesh material */
    void addLink(const u32 &t_meshId, const u32 &t_materialId);

    const u8 &isNameSet() const { return _isNameSet; };

    const u8 &isSizeSet() const { return _isSizeSet; };

    const u8 isLinkedWith(const u32 &t_meshId, const u32 &t_materialId) const
    {
        s32 index = getIndexOfLink(t_meshId, t_materialId);
        if (index != -1)
            return true;
        else
            return false;
    };

    void removeLink(const u32 &t_index) { texLinks.erase(texLinks.begin() + t_index); }

    void removeLink(const u32 &t_meshId, const u32 &t_materialId)
    {
        s32 index = getIndexOfLink(t_meshId, t_materialId);
        if (index != -1)
            removeLink(index);
        else
            PRINT_ERR("Cant remove link, because it was not found!");
    }

private:
    void setDefaultWrapSettings();
    texwrap_t wrapSettings;
    char *name;
    std::vector<TextureLink> texLinks;
    u32 id;
    u8 width, height, _isNameSet, _isSizeSet;
    unsigned char *data;
};

#endif
