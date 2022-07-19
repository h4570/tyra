
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tamtypes.h>
#include <string>
#include <cstdlib>
#include "renderer/3d/mesh/mesh_material.hpp"

namespace Tyra {

MeshMaterial::MeshMaterial(const MeshBuilderData& data,
                           const u32& materialIndex)
    : singleColor(false) {
  TYRA_ASSERT(materialIndex < data.materialsCount && materialIndex >= 0,
              "Provided index \"", materialIndex, "\" is out of range");

  id = rand() % 1000000;

  vertexFaces = data.materials[materialIndex]->vertexFaces;
  TYRA_ASSERT(vertexFaces != nullptr, "Vertex faces are required");

  if (data.textureCoordsEnabled) {
    textureCoordFaces = data.materials[materialIndex]->textureCoordFaces;
    TYRA_ASSERT(textureCoordFaces != nullptr,
                "Texture coord faces are required");
  } else {
    textureCoordFaces = nullptr;
  }

  if (data.normalsEnabled) {
    normalFaces = data.materials[materialIndex]->normalFaces;
    TYRA_ASSERT(normalFaces != nullptr, "Normal faces are required");
  } else {
    normalFaces = nullptr;
  }

  if (data.manyColorsEnabled) {
    colorFaces = data.materials[materialIndex]->colorFaces;
    singleColorFlag = false;
    TYRA_ASSERT(colorFaces != nullptr, "Colors faces are required");
  } else {
    colorFaces = nullptr;
    singleColorFlag = true;
  }

  singleColor.set(128.0F, 128.0F, 128.0F, 128.0F);

  facesCount = data.materials[materialIndex]->count;
  TYRA_ASSERT(facesCount > 0, "Faces count must be greater than 0");

  _name = data.materials[materialIndex]->name;
  TYRA_ASSERT(_name.length() > 0, "MeshMaterial name cannot be empty");

  framesCount = data.framesCount;
  frames = new MeshMaterialFrame*[framesCount];
  for (u32 i = 0; i < framesCount; i++) {
    frames[i] = new MeshMaterialFrame(data, i, materialIndex);
  }

  _isMother = true;
}

MeshMaterial::MeshMaterial(const MeshMaterial& mesh) {
  id = rand() % 1000000;

  vertexFaces = mesh.vertexFaces;
  textureCoordFaces = mesh.textureCoordFaces;
  normalFaces = mesh.normalFaces;
  colorFaces = mesh.colorFaces;

  facesCount = mesh.facesCount;
  framesCount = mesh.framesCount;
  _name = mesh._name;

  singleColor.set(128.0F, 128.0F, 128.0F, 128.0F);

  frames = new MeshMaterialFrame*[framesCount];
  for (u32 i = 0; i < framesCount; i++) {
    frames[i] = new MeshMaterialFrame(*mesh.frames[i]);
  }

  _isMother = false;
}

MeshMaterial::~MeshMaterial() {
  if (_isMother) {
    delete[] vertexFaces;
    if (textureCoordFaces) delete[] textureCoordFaces;
    if (normalFaces) delete[] normalFaces;
    if (colorFaces) delete[] colorFaces;
  }

  for (u32 i = 0; i < framesCount; i++) {
    delete frames[i];
  }
  delete[] frames;
}

const BBox& MeshMaterial::getBBox(const u32& frame) const {
  return frames[frame]->getBBox();
}

void MeshMaterial::setSingleColorFlag(const u8& flag) {
  TYRA_ASSERT(
      colorFaces != nullptr,
      "Colors and color faces are required to use color-per-vertex mode");

  singleColorFlag = flag;
}

void MeshMaterial::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void MeshMaterial::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string MeshMaterial::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "MeshMaterial(";
  }

  res << std::endl;
  res << std::fixed << std::setprecision(2);
  res << "Id: " << id << ", " << std::endl;
  res << "Name: " << _name << ", " << std::endl;
  res << "FacesCount: " << facesCount << ", " << std::endl;
  res << "FramesCount: " << framesCount << ", " << std::endl;

  res << "vertexFaces: " << std::endl;
  for (u32 i = 0; i < facesCount; i++) {
    res << vertexFaces[i] << ", ";
    if (i % 3 == 2) res << std::endl;
  }

  if (textureCoordFaces) {
    res << "TextureCoordFaces: " << std::endl;
    for (u32 i = 0; i < facesCount; i++) {
      res << textureCoordFaces[i] << ", ";
      if (i % 3 == 2) res << std::endl;
    }
  }

  if (normalFaces) {
    res << "NormalFaces: " << std::endl;
    for (u32 i = 0; i < facesCount; i++) {
      res << normalFaces[i] << ", ";
      if (i % 3 == 2) res << std::endl;
    }
  }

  if (colorFaces) {
    res << "ColorFaces: " << std::endl;
    for (u32 i = 0; i < facesCount; i++) {
      res << colorFaces[i] << ", ";
      if (i % 3 == 2) res << std::endl;
    }
  }

  res << ")";

  return res.str();
}

}  // namespace Tyra
