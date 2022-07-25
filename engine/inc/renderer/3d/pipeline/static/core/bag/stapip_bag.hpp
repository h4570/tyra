/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "math/vec4.hpp"
#include "renderer/3d/pipeline/shared/bag/pipeline_info_bag.hpp"
#include "./stapip_color_bag.hpp"
#include "./stapip_lighting_bag.hpp"
#include "./stapip_texture_bag.hpp"
#include "renderer/core/texture/models/texture.hpp"
#include "./packaging/stapip_bag_packages_bbox.hpp"

namespace Tyra {

/**
 * @brief 3D Render data bag.
 * Supports frustum culling, full plane clipping, lighting,
 * texture and single color / many colors.
 */
class StaPipBag {
 public:
  StaPipBag();
  ~StaPipBag();

  /** Mandatory. Object info. */
  PipelineInfoBag* info;

  /** Mandatory. Object color(s). */
  StaPipColorBag* color;

  /** Mandatory. Vertex count. */
  u32 count;

  /** Mandatory. Vertices. */
  Vec4* vertices;

  /** Optional. Texture coordinates and image. */
  StaPipTextureBag* texture;

  /** Optional. Object lighting. */
  StaPipLightingBag* lighting;

  /**
   * @param maxVertCount This parameter is available in renderer API.
   */
  StaPipBagPackagesBBox calculateBbox(const u32& maxVertCount);

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;
};

}  // namespace Tyra
