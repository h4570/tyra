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

    /** 
     * Auto generated unique Id. 
     * Core role of this variable is to dont send 
     * again texture in renderer, when last sent texture id
     * by renderer will be equal to this id.
     */
    const u32 &getId() const { return id; };

    /**
     * Core role of this variable is to assign texture to correct material.
     * For example you can have two textures for single material.
     * Example: Same player can have two textures (team A, team B)
     */
    const u32 &getMaterialId() const { return materialId; };

    const u8 &getWidth() const { return width; };

    const u8 &getHeight() const { return height; };

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

    // ----
    //  Setters
    // ----

    /**
     * Core role of this variable is to assign texture to correct material.
     * For example you can have two textures for single material.
     * Example: Same player can have two textures (team A, team B)
     */
    void setMaterialId(const u32 &t_val) { materialId = t_val; }

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

    const u8 &isNameSet() const { return _isNameSet; };

    const u8 &isSizeSet() const { return _isSizeSet; };

private:
    void setDefaultWrapSettings();

    texwrap_t wrapSettings;
    char *name;
    u32 id, materialId;
    u8 width, height, _isNameSet, _isSizeSet;
    unsigned char *data;
};

#endif
