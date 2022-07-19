
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
#include <string>

namespace Tyra {

/**
 * Data is not deallocated here.
 * Ownership is moved to Mesh class.
 */
class MeshBuilderMaterialData {
 public:
  MeshBuilderMaterialData();
  ~MeshBuilderMaterialData();

  std::string name;
  u32 *vertexFaces, *textureCoordFaces, *normalFaces, *colorFaces;
  u32 count;

  void allocateFaces(const u32& count);
};

}  // namespace Tyra
