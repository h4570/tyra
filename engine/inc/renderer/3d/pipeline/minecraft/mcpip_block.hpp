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

#include <tamtypes.h>
#include "math/m4x4.hpp"
#include "math/vec4.hpp"
#include "renderer/models/color.hpp"

namespace Tyra {

class McpipBlock {
 public:
  McpipBlock() {}
  ~McpipBlock() {}

  M4x4 model;
  Color color;
  Vec4 textureOffset;
};

}  // namespace Tyra
