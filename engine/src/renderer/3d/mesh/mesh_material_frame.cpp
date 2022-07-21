
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
#include <iomanip>
#include <string>
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

  allocateVertices(data, frameIndex, materialIndex);
  allocateNormals(data, frameIndex, materialIndex);
  allocateTextureCoords(data, frameIndex, materialIndex);
  allocateColors(data, frameIndex, materialIndex);

  _isMother = true;
}

MeshMaterialFrame::MeshMaterialFrame(const MeshMaterialFrame& frame) {
  id = rand() % 1000000;

  vertexCount = frame.vertexCount;

  vertices = frame.vertices;
  normals = frame.normals;
  textureCoords = frame.textureCoords;
  colors = frame.colors;

  bbox = frame.bbox;

  _isMother = false;
}

MeshMaterialFrame::~MeshMaterialFrame() {
  if (_isMother) {
    delete[] vertices;
    if (normals) delete[] normals;
    if (textureCoords) delete[] textureCoords;
    if (colors) delete[] colors;
    delete bbox;
  }
}

std::string MeshMaterialFrame::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "MeshMaterialFrame(";
  }
  res << std::fixed << std::setprecision(2);

  res << "Id: " << id << ", " << std::endl;
  res << "Vertex count: " << vertexCount << ", " << std::endl;
  res << "BBox: " << bbox->getPrint() << ", " << std::endl;

  res << "Vertices: ";
  for (u32 i = 0; i < vertexCount; i++) {
    res << vertices[i].getPrint() << ", " << std::endl;
  }

  if (normals) {
    res << "Normals: ";
    for (u32 i = 0; i < vertexCount; i++) {
      res << normals[i].getPrint() << ", " << std::endl;
    }
  }

  if (textureCoords) {
    res << "TextureCoords: ";
    for (u32 i = 0; i < vertexCount; i++) {
      res << textureCoords[i].getPrint() << ", " << std::endl;
    }
  }

  if (colors) {
    res << "Colors: ";
    for (u32 i = 0; i < vertexCount; i++) {
      res << colors[i].getPrint() << ", " << std::endl;
    }
  }

  res << ")";

  return res.str();
}

void MeshMaterialFrame::allocateVertices(const MeshBuilderData& data,
                                         const u32& frameIndex,
                                         const u32& materialIndex) {
  vertexCount = data.materials[materialIndex]->count;  // faces count
  vertices = new Vec4[vertexCount];

  TYRA_ASSERT(vertexCount > 0, "Vertex count must be greater than 0");

  auto* rolled = data.frames[frameIndex]->vertices;
  auto* faces = data.materials[materialIndex]->vertexFaces;

  TYRA_ASSERT(rolled != nullptr, "Vertices are required");
  TYRA_ASSERT(faces != nullptr, "Vertex faces are required");

  for (u32 i = 0; i < vertexCount; i++) vertices[i] = rolled[faces[i]];
}

void MeshMaterialFrame::allocateTextureCoords(const MeshBuilderData& data,
                                              const u32& frameIndex,
                                              const u32& materialIndex) {
  textureCoords = nullptr;
  if (!data.textureCoordsEnabled) return;

  textureCoords = new Vec4[vertexCount];

  auto* rolled = data.frames[frameIndex]->textureCoords;
  auto* faces = data.materials[materialIndex]->textureCoordFaces;

  TYRA_ASSERT(rolled != nullptr, "Texture coordinates are required");
  TYRA_ASSERT(faces != nullptr, "Texture coordinate faces are required");

  for (u32 i = 0; i < vertexCount; i++) textureCoords[i] = rolled[faces[i]];
}

void MeshMaterialFrame::allocateNormals(const MeshBuilderData& data,
                                        const u32& frameIndex,
                                        const u32& materialIndex) {
  normals = nullptr;
  if (!data.normalsEnabled) return;

  normals = new Vec4[vertexCount];

  auto* rolled = data.frames[frameIndex]->normals;
  auto* faces = data.materials[materialIndex]->normalFaces;

  TYRA_ASSERT(rolled != nullptr, "Normals are required");
  TYRA_ASSERT(faces != nullptr, "Normal faces are required");

  for (u32 i = 0; i < vertexCount; i++) normals[i] = rolled[faces[i]];
}

void MeshMaterialFrame::allocateColors(const MeshBuilderData& data,
                                       const u32& frameIndex,
                                       const u32& materialIndex) {
  colors = nullptr;
  if (!data.manyColorsEnabled) return;

  colors = new Color[vertexCount];

  auto* rolled = data.frames[frameIndex]->colors;
  auto* faces = data.materials[materialIndex]->colorFaces;

  TYRA_ASSERT(rolled != nullptr, "Colors are required");
  TYRA_ASSERT(faces != nullptr, "Color faces are required");

  for (u32 i = 0; i < vertexCount; i++) colors[i] = rolled[faces[i]];
}

}  // namespace Tyra
