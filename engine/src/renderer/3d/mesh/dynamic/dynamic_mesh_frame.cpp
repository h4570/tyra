
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
#include <string>
#include "math/vec4.hpp"
#include "renderer/models/color.hpp"
#include "renderer/3d/mesh/dynamic/dynamic_mesh_frame.hpp"

namespace Tyra {

DynamicMeshFrame::DynamicMeshFrame(const MeshBuilderData& data,
                                   const u32& index) {
  TYRA_ASSERT(index < data.framesCount && index >= 0, "Provided index \"",
              index, "\" is out of range");

  id = rand() % 1000000;

  vertices = data.frames[index]->vertices;
  TYRA_ASSERT(vertices != nullptr, "Vertices are required");
  vertexCount = data.frames[index]->verticesCount;
  TYRA_ASSERT(vertexCount > 0, "Vertices count must be greater than 0");

  if (data.normalsEnabled) {
    normals = data.frames[index]->normals;
    normalsCount = data.frames[index]->normalsCount;
    TYRA_ASSERT(normals != nullptr, "Normals are required");
  } else {
    normalsCount = 0;
    normals = nullptr;
  }

  if (data.textureCoordsEnabled) {
    textureCoords = data.frames[index]->textureCoords;
    textureCoordsCount = data.frames[index]->textureCoordsCount;
    TYRA_ASSERT(textureCoords != nullptr, "Texture coordinates are required");
  } else {
    textureCoordsCount = 0;
    textureCoords = nullptr;
  }

  if (data.manyColorsEnabled) {
    colors = data.frames[index]->colors;
    colorsCount = data.frames[index]->colorsCount;
    TYRA_ASSERT(colors != nullptr, "Colors are required");
  } else {
    colorsCount = 0;
    colors = nullptr;
  }

  bbox =
      new BBox(data.frames[index]->vertices, data.frames[index]->verticesCount);

  _isMother = true;
}

DynamicMeshFrame::DynamicMeshFrame(const DynamicMeshFrame& frame) {
  id = rand() % 1000000;

  vertices = frame.vertices;
  normals = frame.normals;
  textureCoords = frame.textureCoords;
  colors = frame.colors;

  vertexCount = frame.vertexCount;
  normalsCount = frame.normalsCount;
  textureCoordsCount = frame.textureCoordsCount;
  colorsCount = frame.colorsCount;

  bbox = frame.bbox;

  _isMother = false;
}

DynamicMeshFrame::~DynamicMeshFrame() {
  if (_isMother) {
    delete[] vertices;
    if (normals) delete[] normals;
    if (textureCoords) delete[] textureCoords;
    if (colors) delete[] colors;
    delete bbox;
  }
}

void DynamicMeshFrame::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void DynamicMeshFrame::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string DynamicMeshFrame::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "MeshFrame(";
  }
  res << std::fixed << std::setprecision(2);

  res << "Id: " << id << ", " << std::endl;
  res << "VertexCount: " << vertexCount << ", " << std::endl;
  res << "NormalsCount: " << normalsCount << ", " << std::endl;
  res << "TextureCoordsCount: " << textureCoordsCount << ", " << std::endl;
  res << "ColorsCount: " << colorsCount << ", " << std::endl;
  res << "BBox: " << bbox->getPrint() << ", " << std::endl;

  res << "Vertices: ";
  for (u32 i = 0; i < vertexCount; i++) {
    res << vertices[i].getPrint() << ", " << std::endl;
  }

  if (normals) {
    res << "Normals: ";
    for (u32 i = 0; i < normalsCount; i++) {
      res << normals[i].getPrint() << ", " << std::endl;
    }
  }

  if (textureCoords) {
    res << "TextureCoords: ";
    for (u32 i = 0; i < textureCoordsCount; i++) {
      res << textureCoords[i].getPrint() << ", " << std::endl;
    }
  }

  if (colors) {
    res << "Colors: ";
    for (u32 i = 0; i < colorsCount; i++) {
      res << colors[i].getPrint() << ", " << std::endl;
    }
  }

  res << ")";

  return res.str();
}

}  // namespace Tyra
