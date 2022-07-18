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

#include "renderer/models/color.hpp"
#include "math/m4x4.hpp"
#include "math/vec4.hpp"

namespace Tyra {

enum StapipLightingBagMode { Auto, Manual };

class StapipLightingBag {
 public:
  explicit StapipLightingBag(const bool& manual = false);
  ~StapipLightingBag();

  Vec4* normals;
  M4x4* lightMatrix;

  void setAmbientColor(const Color& color);
  void setDirectionalLightColors(Color* colors, const u8& count);
  void setDirectionalLightDirections(Vec4* directions, const u8& count);
  void setDirectionalLightColor(const Color& color, const u8& index);
  void setDirectionalLightDirection(const Vec4& direction, const u8& index);

  void setLightsManually(Vec4* colors, Vec4* directions);
  void disableManualMode();

  Vec4* getLightColors() { return lightColors; }

  Vec4* getLightDirections() { return lightDirections; }

  void forceDeallocateColors();
  void forceDeallocateDirections();

 private:
  void allocate();
  void deallocate();
  void forceDeallocate();
  u8 isAllocated;
  StapipLightingBagMode mode;

  Vec4* lightColors;
  Vec4* lightDirections;
};

}  // namespace Tyra
