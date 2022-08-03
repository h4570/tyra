
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "loaders/3d/builder/mesh_builder_material_data.hpp"
#include "debug/debug.hpp"

namespace Tyra {

MeshBuilderMaterialData::MeshBuilderMaterialData() {
  vertexFaces = nullptr;
  textureCoordFaces = nullptr;
  normalFaces = nullptr;
  colorFaces = nullptr;
  name = "";
  count = 0;
}

MeshBuilderMaterialData::~MeshBuilderMaterialData() {
  if (vertexFaces) {
    delete[] vertexFaces;
  }
  if (textureCoordFaces) {
    delete[] textureCoordFaces;
  }
  if (normalFaces) {
    delete[] normalFaces;
  }
  if (colorFaces) {
    delete[] colorFaces;
  }
}

void MeshBuilderMaterialData::allocateFaces(const u32& t_count) {
  TYRA_ASSERT(vertexFaces == nullptr && textureCoordFaces == nullptr &&
                  normalFaces == nullptr && colorFaces == nullptr,
              "Faces are already allocated");
  vertexFaces = new u32[t_count];
  textureCoordFaces = new u32[t_count];
  normalFaces = new u32[t_count];
  colorFaces = new u32[t_count];
  count = t_count;
}

}  // namespace Tyra
