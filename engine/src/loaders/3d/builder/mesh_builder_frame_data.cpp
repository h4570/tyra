
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "loaders/3d/builder/mesh_builder_frame_data.hpp"

namespace Tyra {

MeshBuilderFrameData::MeshBuilderFrameData() {
  vertices = nullptr;
  normals = nullptr;
  textureCoords = nullptr;
  colors = nullptr;
  verticesCount = 0;
  textureCoordsCount = 0;
  normalsCount = 0;
  colorsCount = 0;
}

MeshBuilderFrameData::~MeshBuilderFrameData() {
  if (vertices) {
    delete[] vertices;
  }
  if (normals) {
    delete[] normals;
  }
  if (textureCoords) {
    delete[] textureCoords;
  }
  if (colors) {
    delete[] colors;
  }
}

void MeshBuilderFrameData::allocateTextureCoords(const u32& count) {
  textureCoordsCount = count;
  textureCoords = new Vec4[count];
}

void MeshBuilderFrameData::allocateVertices(const u32& count) {
  verticesCount = count;
  vertices = new Vec4[count];
}

void MeshBuilderFrameData::allocateNormals(const u32& count) {
  normalsCount = count;
  normals = new Vec4[count];
}

void MeshBuilderFrameData::allocateColors(const u32& count) {
  colorsCount = count;
  colors = new Color[count];
}

}  // namespace Tyra
