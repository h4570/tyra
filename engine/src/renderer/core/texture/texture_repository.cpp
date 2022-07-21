/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
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
    if (t_id == textures[i]->getId()) return textures[i];
  return nullptr;
}

const s32 TextureRepository::getIndexOf(const u32& t_texId) const {
  for (u32 i = 0; i < textures.size(); i++)
    if (textures[i]->getId() == t_texId) return i;
  return -1;
}

Texture* TextureRepository::add(Texture* texture) {
  textures.push_back(texture);
  return texture;
}

Texture* TextureRepository::add(const char* fullpath) {
  TextureLoader& loader = texLoaderSelector.getLoaderByFileName(fullpath);

  auto* data = loader.load(fullpath);
  Texture* texture = new Texture(data);
  delete data;

  textures.push_back(texture);
  return texture;
}

void TextureRepository::addByMesh(DynamicMesh* mesh, const char* directory,
                                  const char* extension) {
  auto& loader = texLoaderSelector.getLoaderByExtension(extension);

  std::string dirFixed = directory;
  if (dirFixed.back() != '/' && dirFixed.back() != ':') dirFixed += "/";

  for (u32 i = 0; i < mesh->getMaterialsCount(); i++) {
    std::string fullPath =
        dirFixed + mesh->getMaterial(i)->getName() + "." + extension;

    auto* data = loader.load(fullPath.c_str());
    Texture* texture = new Texture(data);
    delete data;

    texture->addLink(mesh->getMaterial(i)->getId());
    textures.push_back(texture);
  }
}

}  // namespace Tyra
