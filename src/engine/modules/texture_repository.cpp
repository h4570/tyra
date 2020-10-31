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

MeshTexture *TextureRepository::add(char *t_subfolder, char *t_name)
{
    MeshTexture *texture = new MeshTexture();
    loader.load(*texture, t_subfolder, t_name, ".bmp");
    texture->setName(t_name);
    textures.push_back(texture);
    return texture;
}

void TextureRepository::addByMesh(char *t_path, Mesh &mesh)
{
    for (u32 i = 0; i < mesh.getMaterialsCount(); i++)
    {
        MeshTexture *texture = new MeshTexture();
        loader.load(*texture, t_path, mesh.getMaterial(i).getName(), ".bmp");
        texture->setName(mesh.getMaterial(i).getName());
        texture->addLink(mesh.id, mesh.getMaterial(i).getId());
        textures.push_back(texture);
    }
}
