/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <tamtypes.h>
#include <vector>
#include <draw_sampling.h>
#include "./texture_link.hpp"
#include "./texture_wrap.hpp"
#include "../texture_data.hpp"
#include "loaders/texture/builder/texture_builder_data.hpp"
#include "debug/debug.hpp"

namespace Tyra {

/**
 * Class which contains texture data.
 * Textures are paired with meshes/sprites via addLink() and
 * removeLink() functions which use meshId/spriteId and materialId (for mesh).
 */
class Texture {
 public:
  Texture(TextureBuilderData* data);
  ~Texture();

  u32 id;
  std::string name;
  TextureData* core;
  TextureData* clut;

  /** Array of texture links with sprites/meshes */
  std::vector<TextureLink> links;

  inline const int& getWidth() const { return core->width; }

  inline const int& getHeight() const { return core->height; }

  float getSizeInMB() const;

  inline const texwrap_t* getWrapSettings() const { return &wrap; }

  inline const std::vector<TextureLink>& getTextureLinks() const {
    return links;
  }

  /**
   * Returns index of link.
   * -1 if not found.
   * @param t_id
   * For 3D: MeshMaterial id.
   * For 2D: Sprite id.
   */
  const s32 getIndexOfLink(const u32& t_id) const;

  /** Set texture wrapping */
  void setWrapSettings(const TextureWrap t_horizontal,
                       const TextureWrap t_vertical, int minu = 0, int minv = 0,
                       int maxu = 0, int maxv = 0);

  // ----
  //  Other
  // ----

  /** Assign texture to Sprite.Id or MeshMaterial.Id. */
  void addLink(const u32& t_id);

  u32 getTextureSize() const;

  /**
   * Check if texture is linked with MeshMaterial/Sprite.
   * @param t_id
   * For 3D: MeshMaterial material id.
   * For 2D: Sprite id.
   */
  const u8 isLinkedWith(const u32& t_id) const;

  void removeLinkByIndex(const u32& t_index);

  /**
   * Remove texture link with given MeshMaterial/Sprite.
   * @param t_id
   * For 3D: MeshMaterial material id.
   * For 2D: Sprite id.
   */
  void removeLinkById(const u32& t_id);

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

  void setDefaultWrapSettings();

 private:
  void setPsm();

  texwrap_t wrap;
};
}  // namespace Tyra
