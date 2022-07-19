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
#include "renderer/models/color.hpp"

namespace Tyra {

class StdpipLightingOptions {
 public:
  StdpipLightingOptions() {}
  ~StdpipLightingOptions() {}

  /**
   * Mandatory.
   * Default 128.0F, 128.0F, 128.0F, 128.0F
   */
  Color* ambientColor;

  /**
   * Mandatory.
   * Min/max length - 3.
   * Example color value: 64.0F, 0.0F, 0.0F, 1.0F
   */
  Color* directionalColors;

  /**
   * Mandatory.
   * Min/max length - 3.
   * Example dir value: 1.0F, 0.0F, 0.0F, 1.0F
   */
  Vec4* directionalDirections;
};

}  // namespace Tyra
