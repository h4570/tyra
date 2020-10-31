/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/models/mesh_texture.hpp"
#include "../include/utils/string.hpp"
#include "../include/utils/debug.hpp"
#include <cstdlib>
#include <draw_sampling.h>

// ----
// Constructors/Destructors
// ----

MeshTexture::MeshTexture()
{
    id = rand() % 1000000;
    _isSizeSet = false;
    _isNameSet = false;
    setDefaultWrapSettings();
}

MeshTexture::~MeshTexture()
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

void MeshTexture::setSize(const u8 &t_width, const u8 &t_height)
{
    if (_isSizeSet)
    {
        PRINT_ERR("Can't set size, because was already set!");
        return;
    }
    width = t_width;
    height = t_height;
    data = new unsigned char[getDataSize()];
    _isSizeSet = true;
}

void MeshTexture::setName(char *t_val)
{
    if (_isNameSet)
    {
        PRINT_ERR("Can't set name, because was already set!");
        return;
    }
    name = String::createCopy(t_val);
    _isNameSet = true;
}

void MeshTexture::setDefaultWrapSettings()
{
    wrapSettings.horizontal = WRAP_REPEAT;
    wrapSettings.vertical = WRAP_REPEAT;
    wrapSettings.maxu = 0;
    wrapSettings.maxv = 0;
    wrapSettings.minu = 0;
    wrapSettings.minv = 0;
}

void MeshTexture::setWrapSettings(const WrapSettings t_horizontal, const WrapSettings t_vertical)
{
    wrapSettings.horizontal = t_horizontal;
    wrapSettings.vertical = t_vertical;
}

void MeshTexture::addLink(const u32 &t_meshId, const u32 &t_materialId)
{
    TextureLink link;
    link.materialId = t_materialId;
    link.meshId = t_meshId;
    texLinks.push_back(link);
}
