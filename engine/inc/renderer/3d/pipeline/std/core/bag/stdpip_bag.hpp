/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "math/vec4.hpp"
#include "./stdpip_lighting_bag.hpp"
#include "./stdpip_info_bag.hpp"
#include "./stdpip_color_bag.hpp"
#include "./stdpip_texture_bag.hpp"
#include "renderer/core/texture/models/texture.hpp"
#include "./packaging/stdpip_bag_packages_bbox.hpp"

namespace Tyra {

/**
 * @brief 3D Render data bag.
 * Supports frustum culling, clipping, lighting,
 * texture and single color / many colors.
 */
class StdpipBag {
 public:
  StdpipBag();
  ~StdpipBag();

  /** Mandatory. Object info. */
  StdpipInfoBag* info;

  /** Mandatory. Object color(s). */
  StdpipColorBag* color;

  /** Mandatory. Vertex count. */
  u32 count;

  /** Mandatory. Vertices. */
  Vec4* vertices;

  /** Optional. Texture coordinates and image. */
  StdpipTextureBag* texture;

  /** Optional. Object lighting. */
  StdpipLightingBag* lighting;

  /**
   * @param maxVertCount This parameter is available in renderer API.
   */
  StdpipBagPackagesBBox calculateBbox(const u32& maxVertCount);

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;
};

}  // namespace Tyra
