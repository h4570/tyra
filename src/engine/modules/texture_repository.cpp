/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/modules/texture_repository.hpp"
#include "../include/utils/debug.hpp"

// ----
// Constructors/Destructors
// ----

TextureRepository::TextureRepository()
{
    PRINT_LOG("Initializing texture repository");
    PRINT_LOG("Texture repository initialized!");
}

TextureRepository::~TextureRepository()
{
    if (getTexturesCount() > 0)
    {
        for (u32 i = 0; i < getTexturesCount(); i++)
            delete textures[i];
        textures.clear();
    }
}

// ----
// Methods
// ----

Texture *TextureRepository::add(char *t_subfolder, char *t_name, TextureFormat t_format)
{
    Texture *texture = new Texture();
    if (t_format == BMP)
        bmpLoader.load(*texture, t_subfolder, t_name, ".bmp");
    else
        pngLoader.load(*texture, t_subfolder, t_name, ".png");
    texture->setName(t_name);
    textures.push_back(texture);
    return texture;
}

void TextureRepository::addByMesh(char *t_path, Mesh &mesh, TextureFormat t_format)
{
    for (u32 i = 0; i < mesh.getMaterialsCount(); i++)
    {
        Texture *texture = new Texture();
        if (t_format == BMP)
            bmpLoader.load(*texture, t_path, mesh.getMaterial(i).getName(), ".bmp");
        else
            pngLoader.load(*texture, t_path, mesh.getMaterial(i).getName(), ".png");
        texture->setName(mesh.getMaterial(i).getName());
        texture->addLink(mesh.getId(), mesh.getMaterial(i).getId());
        textures.push_back(texture);
    }
}
