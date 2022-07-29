
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

#include "./mesh_builder_material_data.hpp"
#include "./mesh_builder_frame_data.hpp"

namespace Tyra {

class MeshBuilderData {
 public:
  MeshBuilderData();

  /**
   * Frames array and materials array will be deallocated, but
   * data (vert,st, ...) inside it not.
   * Data ownership is transferred to
   * the Mesh class.
   */
  ~MeshBuilderData();

  MeshBuilderFrameData** frames;
  u32 framesCount;

  MeshBuilderMaterialData** materials;
  u32 materialsCount;

  bool textureCoordsEnabled, normalsEnabled, manyColorsEnabled;

  void allocateFrames(const u32& count);
  void allocateMaterials(const u32& count);
  void allocate(const u32& framesCount, const u32& materialsCount);
};

}  // namespace Tyra
