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
#include "./dynpip_color_bag.hpp"
#include "./dynpip_lighting_bag.hpp"
#include "./dynpip_texture_bag.hpp"
#include "renderer/core/texture/models/texture.hpp"

namespace Tyra {

/**
 * @brief 3D Animated render data bag.
 * Supports frustum culling, simple clipping (culling), lighting,
 * texture and single color / many colors.
 */
class DynPipBag {
 public:
  DynPipBag();
  ~DynPipBag();

  /** Mandatory. Object info. */
  PipelineInfoBag* info;

  /** Mandatory. Object color(s). */
  DynPipColorBag* color;

  /** Mandatory. Vertex count per frame. */
  u32 count;

  /** Mandatory. From (frame) vertices */
  Vec4* verticesFrom;

  /** Mandatory. To (frame) vertices */
  Vec4* verticesTo;

  /**
   * Mandatory.
   * State of animation interpolation
   * (between from and to)
   */
  float interpolation;

  /** Optional. Texture coordinates and image. */
  DynPipTextureBag* texture;

  /** Optional. Object lighting. */
  DynPipLightingBag* lighting;

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;
};

}  // namespace Tyra
