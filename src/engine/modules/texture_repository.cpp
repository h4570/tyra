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
    texturesCount = 0;
    PRINT_LOG("Texture repository initialized!");
}

TextureRepository::~TextureRepository() {}

// ----
// Methods
// ----

MeshTexture *TextureRepository::add(char *t_subfolder, char *t_name)
{
    increaseArray(texturesCount + 1);
    // texturesCount is updated
    loader.load(*textures[texturesCount - 1], t_subfolder, t_name, ".bmp");
    return textures[texturesCount - 1];
}

void TextureRepository::addByMesh(char *t_path, Mesh &mesh)
{
#define MATERIAL mesh.getMaterial(i)
    u32 newLength = texturesCount + mesh.getMaterialsCount();
    increaseArray(newLength);
    for (u8 i = newLength - mesh.getMaterialsCount(); i < newLength; i++)
    {
        textures[i] = new MeshTexture();
        loader.load(*textures[i], t_path, MATERIAL.getName(), ".bmp");
        textures[i]->setName(mesh.getMaterial(i).getName());
        textures[i]->addLink(mesh.id, MATERIAL.getId());
    }
}

/** 
 * Remove texture from repository.
 * Texture is not destructed.
 */
void TextureRepository::remove(u32 t_textureId)
{
    MeshTexture **savedTextures = textures;
    textures = new MeshTexture *[texturesCount - 1];
    u32 savedCount = 0;
    for (u32 i = 0; i < texturesCount; i++)
        if (savedTextures[i]->getId() != t_textureId)
            textures[savedCount++] = savedTextures[i];
    delete[] savedTextures;
    texturesCount--;
}

/** 
 * Resize textures array. 
 * texturesCount variable is updated. 
 * Classes are NOT instantiated. 
 */
void TextureRepository::increaseArray(u32 newLength)
{
    MeshTexture **savedTextures = textures;
    textures = new MeshTexture *[newLength];
    if (texturesCount)
    {
        for (u32 i = 0; i < texturesCount; i++)
            textures[i] = savedTextures[i];
        delete[] savedTextures;
    }
    texturesCount = newLength;
}
