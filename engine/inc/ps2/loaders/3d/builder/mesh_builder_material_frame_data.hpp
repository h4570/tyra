
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

namespace Tyra {

class MeshBuilderMaterialFrameData {
 public:
  MeshBuilderMaterialFrameData();
  ~MeshBuilderMaterialFrameData();

  unsigned int count;
  Vec4 *vertices, *textureCoords, *normals;
  Color* colors;
};

}  // namespace Tyra
