/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <tamtypes.h>
#include <draw_buffers.h>
#include <vector>
#include "./models/texture.hpp"
#include "renderer/3d/mesh/mesh.hpp"
#include "loaders/texture/base/texture_loader_selector.hpp"
#include <string>

namespace Tyra {
class TextureRepository {
 public:
  TextureRepository();
  ~TextureRepository();

  /** Returns all repository textures. */
  std::vector<Texture*>* getAll() { return &textures; }

  u32 getTexturesCount() const { return static_cast<u32>(textures.size()); }

  /**
   * Returns single texture.
   * nullptr if not found.
   * @param t_id
   * For 3D: MeshMaterial id.
   * For 2D: Sprite id.
   */
  Texture* getBySpriteOrMesh(const u32& t_id) {
    for (u32 i = 0; i < textures.size(); i++)
      if (textures[i]->isLinkedWith(t_id)) return textures[i];
    return nullptr;
  }

  /**
   * Returns single texture.
   * nullptr if not found.
   */
  Texture* getByTextureId(const u32& t_id) const {
    for (u32 i = 0; i < textures.size(); i++)
      if (t_id == textures[i]->getId()) return textures[i];
    return nullptr;
  }

  /**
   * Returns index of link.
   * -1 if not found.
   */
  const s32 getIndexOf(const u32& t_texId) const {
    for (u32 i = 0; i < textures.size(); i++)
      if (textures[i]->getId() == t_texId) return i;
    return -1;
  }

  // ----
  //  Setters
  // ----

  // ----
  //  Other
  // ----

  /**
   * Add unlinked texture.
   * @param fullpath Full path to texture file. Example: "host:texture.png"
   */
  Texture* add(const char* fullpath);

  /**
   * Add unlinked texture.
   * @param fullpath Full path to texture file. Example: "host:texture.png"
   */
  inline Texture* add(const std::string& fullpath) {
    return add(fullpath.c_str());
  }

  /**
   * Add unlinked texture.
   * @param texture Your own texture
   */
  Texture* add(Texture* texture);

  /**
   * Add linked textures in given path for mesh material names.
   */
  void addByMesh(Mesh* mesh, const char* directory, const char* extension);

  /**
   * Add linked textures in given path for mesh material names.
   */
  inline void addByMesh(Mesh* mesh, const std::string& directory,
                        const char* extension) {
    addByMesh(mesh, directory.c_str(), extension);
  }

  /**
   * Remove texture from repository.
   * Texture is NOT destructed.
   */
  void removeByIndex(const u32& t_index) {
    textures.erase(textures.begin() + t_index);
  }

  /**
   * Remove texture from repository.
   * Texture is NOT destructed.
   */
  const void removeById(const u32& t_texId) {
    s32 index = getIndexOf(t_texId);
    TYRA_ASSERT(index != -1, "Cant remove texture, because it was not found!");
    removeByIndex(index);
  }

 private:
  std::vector<Texture*> textures;
  TextureLoaderSelector texLoaderSelector;
};
}  // namespace Tyra
