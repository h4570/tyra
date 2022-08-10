
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "debug/debug.hpp"
#include <tamtypes.h>
#include <iomanip>
#include <string>
#include "renderer/models/color.hpp"
#include "renderer/3d/mesh/mesh_material_frame.hpp"

namespace Tyra {

MeshMaterialFrame::MeshMaterialFrame(const MeshBuilderData& data,
                                     const u32& frameIndex,
                                     const u32& materialIndex) {
  TYRA_ASSERT(materialIndex < data.materials.size(), "Provided index \"",
              materialIndex, "\" is out of range");

  TYRA_ASSERT(frameIndex < data.materials[materialIndex]->frames.size(),
              "Provided index \"", frameIndex, "\" is out of range");

  id = rand() % 1000000;

  auto* frame = data.materials[materialIndex]->frames[frameIndex];

  TYRA_ASSERT(frame->count > 0, "Vertex count must be greater than 0");
  TYRA_ASSERT(frame->vertices != nullptr, "Vertex array can't be null");
  TYRA_ASSERT(!data.normalsEnabled || frame->normals != nullptr,
              "Normal array can't be null");
  TYRA_ASSERT(!data.textureCoordsEnabled || frame->textureCoords != nullptr,
              "Texture coordinate array can't be null");
  TYRA_ASSERT(!data.lightMapEnabled || frame->colors != nullptr,
              "Color array can't be null");

  bbox = new BBox(frame->vertices, frame->count);

  count = frame->count;
  vertices = frame->vertices;

  if (data.normalsEnabled) {
    normals = frame->normals;
  } else {
    if (frame->normals != nullptr) delete[] frame->normals;

    normals = nullptr;
  }

  if (data.textureCoordsEnabled) {
    textureCoords = frame->textureCoords;
  } else {
    if (frame->textureCoords != nullptr) delete[] frame->textureCoords;

    textureCoords = nullptr;
  }

  if (data.lightMapEnabled) {
    colors = frame->colors;
  } else {
    if (frame->colors != nullptr) delete[] frame->colors;

    colors = nullptr;
  }

  isMother = true;
}

MeshMaterialFrame::MeshMaterialFrame(const MeshMaterialFrame& frame) {
  id = rand() % 1000000;

  count = frame.count;

  vertices = frame.vertices;
  normals = frame.normals;
  textureCoords = frame.textureCoords;
  colors = frame.colors;

  bbox = frame.bbox;

  isMother = false;
}

MeshMaterialFrame::~MeshMaterialFrame() {
  if (isMother) {
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
  res << "Vertex count: " << count << ", " << std::endl;
  res << "BBox: " << bbox->getPrint() << ", " << std::endl;

  res << "Vertices: ";
  for (u32 i = 0; i < count; i++) {
    res << vertices[i].getPrint() << ", " << std::endl;
  }

  if (normals) {
    res << "Normals: ";
    for (u32 i = 0; i < count; i++) {
      res << normals[i].getPrint() << ", " << std::endl;
    }
  }

  if (textureCoords) {
    res << "TextureCoords: ";
    for (u32 i = 0; i < count; i++) {
      res << textureCoords[i].getPrint() << ", " << std::endl;
    }
  }

  if (colors) {
    res << "Colors: ";
    for (u32 i = 0; i < count; i++) {
      res << colors[i].getPrint() << ", " << std::endl;
    }
  }

  res << ")";

  return res.str();
}

}  // namespace Tyra
