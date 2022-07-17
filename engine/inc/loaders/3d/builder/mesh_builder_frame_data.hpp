
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <tamtypes.h>
#include "math/vec4.hpp"
#include "renderer/models/color.hpp"

namespace Tyra {

/**
 * Data is not deallocated here.
 * Ownership is moved to Mesh class.
 */
class MeshBuilderFrameData {
 public:
  MeshBuilderFrameData();
  ~MeshBuilderFrameData();

  Vec4 *vertices, *normals, *textureCoords;
  Color* colors;
  u32 verticesCount, textureCoordsCount, normalsCount, colorsCount;

  void allocateTextureCoords(const u32& count);
  void allocateVertices(const u32& count);
  void allocateNormals(const u32& count);
  void allocateColors(const u32& count);
};

}  // namespace Tyra
