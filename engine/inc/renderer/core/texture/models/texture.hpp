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

  inline const u32& getId() const { return id; }

  inline const int& getWidth() const { return core->width; }

  inline const int& getHeight() const { return core->height; }

  float getSizeInMB() const;

  inline u32 getTextureLinksCount() const {
    return static_cast<u32>(links.size());
  }

  inline texwrap_t* getWrapSettings() { return &wrap; }

  const TextureData& getCoreData() const { return *core; }

  const TextureData* getClutData() const { return clut; }

  /**
   * Get texture name.
   * For "textures/abc.bmp" result will be: "abc"
   */
  inline const std::string& getName() const { return name; }

  /** Array of texture links. Size of getTextureLinksCount() */
  inline const std::vector<TextureLink>& getTextureLinks() const {
    return links;
  }

  /**
   * Returns index of link.
   * -1 if not found.
   * @param t_id
   * For 3D: MeshMaterial material id.
   * For 2D: Sprite id.
   */
  const s32 getIndexOfLink(const u32& t_id) const {
    for (u32 i = 0; i < links.size(); i++)
      if (links[i].id == t_id) return i;
    return -1;
  }

  /** Set texture wrapping */
  void setWrapSettings(const TextureWrap t_horizontal,
                       const TextureWrap t_vertical);

  // ----
  //  Other
  // ----

  /** Assign texture to mesh and mesh material. */
  void addLink(const u32& t_meshId, const u32& t_materialId);

  /** Assign texture to sprite. */
  void addLink(const u32& t_spriteId);

  u32 getTextureSize() const;

  /**
   * Check if texture is linked with MeshMaterial/Sprite.
   * @param t_id
   * For 3D: MeshMaterial material id.
   * For 2D: Sprite id.
   */
  const u8 isLinkedWith(const u32& t_id) const {
    s32 index = getIndexOfLink(t_id);
    if (index != -1)
      return true;
    else
      return false;
  }

  void removeLinkByIndex(const u32& t_index) {
    links.erase(links.begin() + t_index);
  }

  /**
   * Remove texture link with given MeshMaterial/Sprite.
   * @param t_id
   * For 3D: MeshMaterial material id.
   * For 2D: Sprite id.
   */
  void removeLinkById(const u32& t_id) {
    s32 index = getIndexOfLink(t_id);
    TYRA_ASSERT(index != -1, "Cant remove link, because it was not found!");
    removeLinkByIndex(index);
  }

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 private:
  void setPsm();
  void setDefaultWrapSettings();

  u32 id;
  std::string name;

  TextureData* core;
  TextureData* clut;

  texwrap_t wrap;
  std::vector<TextureLink> links;
};
}  // namespace Tyra
