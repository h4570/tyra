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

#include "math/vec4.hpp"
#include "renderer/models/color.hpp"
#include "renderer/core/3d/bbox/core_bbox.hpp"

namespace Tyra {

/** Debug functions, with bad performance. */
class Renderer3DUtility {
 public:
  Renderer3DUtility();
  ~Renderer3DUtility();

  void drawPoint(const Vec4& v);
  void drawPoint(const Vec4& v, const float& size);
  void drawPoint(const Vec4& v, const float& size, const Color& color);

  void drawLine(const Vec4& from, const Vec4& to);
  void drawLine(const Vec4& from, const Vec4& to, const float& size);
  void drawLine(const Vec4& from, const Vec4& to, const float& size,
                const Color& color);

  void drawBBox(const CoreBBox& v);
  void drawBBox(const CoreBBox& v, const float& size);
  void drawBBox(const CoreBBox& v, const float& size, const Color& color);
};

}  // namespace Tyra
