/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_TEXTURE_
#define _TYRA_TEXTURE_

#include <tamtypes.h>
#include <draw_sampling.h>
#include "./texture_wrap_settings.hpp"
#include "./texture_link.hpp"
#include "../include/utils/debug.hpp"
#include <vector>
#include <gs_psm.h>

enum TextureType
{
    TEX_TYPE_RGB = GS_PSM_24, // BMP
    TEX_TYPE_RGBA = GS_PSM_32 // PNG
};

/** 
 * Class which contains texture data.
 * Textures are paired with meshes/sprites via addLink() and 
 * removeLink() functions which use meshId/spriteId and materialId (for mesh).
 */
class Texture
{

public:
    Texture();
    ~Texture();

    // ----
    // Getters
    // ----

    /** 
     * Auto generated unique Id. 
     * Core role of this variable is to dont send 
     * again texture in renderer, when last sent texture id
     * by renderer will be equal to this id.
     */
    inline const u32 getId() const { return id; };

    inline const u16 getWidth() const { return width; };

    inline const u16 getHeight() const { return height; };

    inline const TextureType getType() const { return _type; };

    inline u32 getTextureLinksCount() const { return static_cast<u32>(texLinks.size()); };

    inline texwrap_t *getWrapSettings() { return &wrapSettings; };

    /** 
     * Returns always width * width * 3/4. 
     * 3 for RGB, 4 for RGBA.
     */
    inline u32 getDataSize() const { return _type == TEX_TYPE_RGB ? width * height * 3 : width * height * 4; };

    /** 
     * Texture data, used by renderer.
     * Array of size getDataSize().
     */
    inline unsigned char *getData() const { return data; };

    /** 
     * Get texture name.
     * For "textures/abc.bmp" result will be: "abc"
     */
    inline char *getName() const { return name; };

    /** Array of texture links. Size of getTextureLinksCount() */
    inline const std::vector<TextureLink> &getTextureLinks() const { return texLinks; };

    /** 
     * Returns index of link.
     * -1 if not found.
     */
    const s32 getIndexOfLink(const u32 &t_meshId, const u32 &t_materialId) const
    {
        for (u32 i = 0; i < texLinks.size(); i++)
            if (texLinks[i].materialId == t_materialId && texLinks[i].meshOrSpriteId == t_meshId)
                return i;
        return -1;
    };

    /** 
     * Returns index of link.
     * -1 if not found.
     */
    inline const s32 getIndexOfLink(const u32 &t_spriteId) const { return getIndexOfLink(t_spriteId, 0); };

    // ----
    //  Setters
    // ----

    /** 
     * Set texture size and allocate texture data
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
     */
    void setSize(const u8 &t_width, const u8 &t_height, const TextureType &t_type);

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
     */
    void setHeight(const u8 &t_val) { height = t_val; }

    /** 
     * Do not call this method unless you know what you do.
     * Should be called by data loader. 
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

    /** Assign texture to mesh and mesh material. */
    void addLink(const u32 &t_meshId, const u32 &t_materialId);

    /** Assign texture to sprite. */
    void addLink(const u32 &t_spriteId);

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

    const u8 isLinkedWith(const u32 &t_spriteId) const
    {
        s32 index = getIndexOfLink(t_spriteId);
        if (index != -1)
            return true;
        else
            return false;
    };

    void removeLinkByIndex(const u32 &t_index) { texLinks.erase(texLinks.begin() + t_index); }

    void removeLinkBySprite(const u32 &t_spriteId)
    {
        s32 index = getIndexOfLink(t_spriteId);
        if (index != -1)
            removeLinkByIndex(index);
        else
            PRINT_ERR("Cant remove link, because it was not found!");
    }

    void removeLinkByMesh(const u32 &t_meshId, const u32 &t_materialId)
    {
        s32 index = getIndexOfLink(t_meshId, t_materialId);
        if (index != -1)
            removeLinkByIndex(index);
        else
            PRINT_ERR("Cant remove link, because it was not found!");
    }

private:
    void setDefaultWrapSettings();
    texwrap_t wrapSettings;
    char *name;
    TextureType _type;
    std::vector<TextureLink> texLinks;
    u32 id;
    u8 width, height, _isNameSet, _isSizeSet;
    unsigned char *data;
};

#endif
