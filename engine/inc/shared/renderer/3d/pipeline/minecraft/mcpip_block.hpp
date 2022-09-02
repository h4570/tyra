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

#include "shared/math/m4x4.hpp"
#include "shared/math/vec4.hpp"
#include "shared/renderer/models/color.hpp"

namespace Tyra {

class McpipBlock {
 public:
  McpipBlock() {
    model = nullptr;
    color = nullptr;
    textureOffset = nullptr;
  }
  ~McpipBlock() {}

  M4x4* model;
  Color* color;
  Vec4* textureOffset;
};

}  // namespace Tyra
