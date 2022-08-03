
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
#include "math/vec4.hpp"
#include "renderer/models/color.hpp"

namespace Tyra {

class MeshBuilder2MaterialFrameData {
 public:
  MeshBuilder2MaterialFrameData();
  ~MeshBuilder2MaterialFrameData();

  u32 count;
  Vec4 *vertices, *textureCoords, *normals;
  Color* colors;
};

}  // namespace Tyra
