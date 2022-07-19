
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "loaders/3d/builder/mesh_builder_data.hpp"

namespace Tyra {

MeshBuilderData::MeshBuilderData() {
  normalsEnabled = false;
  textureCoordsEnabled = false;
  manyColorsEnabled = false;
}

MeshBuilderData::~MeshBuilderData() {
  if (frames) {
    for (u32 i = 0; i < framesCount; i++) {
      delete frames[i];
    }
    delete[] frames;
  }

  if (materials) {
    for (u32 i = 0; i < materialsCount; i++) {
      delete materials[i];
    }
    delete[] materials;
  }
}

void MeshBuilderData::allocate(const u32& framesCount,
                               const u32& materialsCount) {
  allocateFrames(framesCount);
  allocateMaterials(materialsCount);
}

void MeshBuilderData::allocateFrames(const u32& count) {
  this->framesCount = count;

  frames = new MeshBuilderFrameData*[count];

  for (u32 i = 0; i < count; i++) {
    frames[i] = new MeshBuilderFrameData();
  }
}

void MeshBuilderData::allocateMaterials(const u32& count) {
  this->materialsCount = count;

  materials = new MeshBuilderMaterialData*[count];

  for (u32 i = 0; i < count; i++) {
    materials[i] = new MeshBuilderMaterialData();
  }
}

}  // namespace Tyra
