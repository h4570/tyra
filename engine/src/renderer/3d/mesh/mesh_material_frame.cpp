
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "debug/debug.hpp"
#include <tamtypes.h>
#include "renderer/models/color.hpp"
#include "loaders/3d/builder/mesh_builder_data.hpp"
#include "renderer/3d/mesh/mesh_material_frame.hpp"

namespace Tyra {

MeshMaterialFrame::MeshMaterialFrame(const MeshBuilderData& data,
                                     const u32& frameIndex,
                                     const u32& materialIndex) {
  TYRA_ASSERT(frameIndex < data.framesCount && frameIndex >= 0,
              "Provided index \"", frameIndex, "\" is out of range");
  TYRA_ASSERT(materialIndex < data.materialsCount && materialIndex >= 0,
              "Provided index \"", materialIndex, "\" is out of range");

  id = rand() % 1000000;

  bbox = new BBox(data.frames[frameIndex]->vertices,
                  data.materials[materialIndex]->vertexFaces,
                  data.materials[materialIndex]->count);

  _isMother = true;
}

MeshMaterialFrame::MeshMaterialFrame(const MeshMaterialFrame& frame) {
  id = rand() % 1000000;

  bbox = frame.bbox;

  _isMother = false;
}

MeshMaterialFrame::~MeshMaterialFrame() {
  if (_isMother) {
    delete bbox;
  }
}

}  // namespace Tyra
