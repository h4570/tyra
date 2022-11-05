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

namespace Tyra {

class McpipBlockData {
 public:
  McpipBlockData();
  ~McpipBlockData();

  u32 count;
  float offset;
  u32 getComboCount() const { return count * 2; }

  Tyra::Vec4* comboData;
  Tyra::Vec4* vertices;
  Tyra::Vec4* textureCoords;
};

}  // namespace Tyra
