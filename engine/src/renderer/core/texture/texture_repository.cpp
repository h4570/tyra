/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/texture/texture_repository.hpp"

namespace Tyra {

TextureRepository::TextureRepository() {}

TextureRepository::~TextureRepository() {
  TYRA_LOG("Texture repository destructed!");
  if (getTexturesCount() > 0) {
    for (u32 i = 0; i < getTexturesCount(); i++) delete textures[i];
    textures.clear();
  }
}

Texture* TextureRepository::getBySpriteOrMesh(const u32& t_id) const {
  for (u32 i = 0; i < textures.size(); i++) {
    if (textures[i]->isLinkedWith(t_id)) return textures[i];
  }
  return nullptr;
}

Texture* TextureRepository::getByTextureId(const u32& t_id) const {
  for (u32 i = 0; i < textures.size(); i++)
    if (t_id == textures[i]->id) return textures[i];
  return nullptr;
}

const s32 TextureRepository::getIndexOf(const u32& t_texId) const {
  for (u32 i = 0; i < textures.size(); i++)
    if (textures[i]->id == t_texId) return i;
  return -1;
}

Texture* TextureRepository::add(Texture* texture) {
  textures.push_back(texture);
  return texture;
}

void TextureRepository::removeByIndex(const u32& t_index) {
  textures.erase(textures.begin() + t_index);
}

void TextureRepository::removeById(const u32& t_texId) {
  s32 index = getIndexOf(t_texId);
  TYRA_ASSERT(index != -1, "Cant remove texture, because it was not found!");
  removeByIndex(index);
}

void TextureRepository::free(const Texture* t_tex) { free(t_tex->id); }

void TextureRepository::free(const Texture& t_tex) { free(t_tex.id); }

void TextureRepository::free(const u32& t_texId) {
  s32 index = getIndexOf(t_texId);
  auto* tex = textures[index];

  TYRA_ASSERT(index != -1, "Cant remove texture, because it was not found!");
  removeByIndex(index);

  delete tex;
}

Texture* TextureRepository::add(const char* fullpath) {
  TextureLoader& loader = texLoaderSelector.getLoaderByFileName(fullpath);

  auto* data = loader.load(fullpath);
  Texture* texture = new Texture(data);
  delete data;

  textures.push_back(texture);
  return texture;
}

void TextureRepository::addByMesh(Mesh* mesh, const char* directory,
                                  const char* extension) {
  auto& loader = texLoaderSelector.getLoaderByExtension(extension);

  std::string dirFixed = directory;
  if (dirFixed.back() != '/' && dirFixed.back() != ':') dirFixed += "/";

  for (u32 i = 0; i < mesh->materials.size(); i++) {
    std::string fullPath =
        dirFixed + mesh->materials[i]->name + "." + extension;

    auto* data = loader.load(fullPath.c_str());
    Texture* texture = new Texture(data);
    delete data;

    texture->addLink(mesh->materials[i]->id);
    textures.push_back(texture);
  }
}

}  // namespace Tyra
