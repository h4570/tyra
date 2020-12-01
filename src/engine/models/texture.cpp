/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/models/texture.hpp"
#include "../include/utils/string.hpp"
#include <cstdlib>
#include <draw_sampling.h>

// ----
// Constructors/Destructors
// ----

Texture::Texture()
{
    id = rand() % 1000000;
    _isSizeSet = false;
    _isNameSet = false;
    setDefaultWrapSettings();
}

Texture::~Texture()
{
    if (getTextureLinksCount() > 0)
        texLinks.clear();
    if (_isNameSet)
        delete[] name;
    if (_isSizeSet)
        delete[] data;
}

// ----
// Methods
// ----

void Texture::setSize(const u8 &t_width, const u8 &t_height, const TextureType &t_type)
{
    if (_isSizeSet)
    {
        PRINT_ERR("Can't set size, because was already set!");
        return;
    }
    if (t_width > 256 || t_height > 256)
        PRINT_ERR("Given texture can be too big for PS2. Please strict to 256x256 max. Prefer 128x128.");
    width = t_width;
    height = t_height;
    _type = t_type;
    data = new unsigned char[getDataSize()];
    _isSizeSet = true;
}

void Texture::setName(char *t_val)
{
    if (_isNameSet)
    {
        PRINT_ERR("Can't set name, because was already set!");
        return;
    }
    name = String::createCopy(t_val);
    _isNameSet = true;
}

void Texture::setDefaultWrapSettings()
{
    wrapSettings.horizontal = WRAP_REPEAT;
    wrapSettings.vertical = WRAP_REPEAT;
    wrapSettings.maxu = 0;
    wrapSettings.maxv = 0;
    wrapSettings.minu = 0;
    wrapSettings.minv = 0;
}

void Texture::setWrapSettings(const WrapSettings t_horizontal, const WrapSettings t_vertical)
{
    wrapSettings.horizontal = t_horizontal;
    wrapSettings.vertical = t_vertical;
}

void Texture::addLink(const u32 &t_meshId, const u32 &t_materialId)
{
    TextureLink link;
    link.materialId = t_materialId;
    link.meshOrSpriteId = t_meshId;
    texLinks.push_back(link);
}

void Texture::addLink(const u32 &t_spriteId)
{
    TextureLink link;
    link.materialId = 0;
    link.meshOrSpriteId = t_spriteId;
    texLinks.push_back(link);
}
