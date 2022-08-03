
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tamtypes.h>
#include <string>
#include <cstdlib>
#include <iomanip>
#include "renderer/3d/mesh/mesh_material.hpp"

namespace Tyra {

MeshMaterial::MeshMaterial(const MeshBuilder2Data& data,
                           const u32& materialIndex) {
  TYRA_ASSERT(materialIndex < data.materials.size(), "Provided index \"",
              materialIndex, "\" is out of range");

  auto* material = data.materials[materialIndex];

  TYRA_ASSERT(material->frames.size() > 0,
              "Material must have at least one frame");

  id = rand() % 1000000;

  if (data.lightMapEnabled) {
    singleColorFlag = false;
    TYRA_ASSERT(material->frames[0]->colors != nullptr,
                "Colors faces are required");
  } else {
    singleColorFlag = true;
  }

  color.set(128.0F, 128.0F, 128.0F, 128.0F);

  _name = material->name;
  TYRA_ASSERT(_name.length() > 0, "MeshMaterial name cannot be empty");

  framesCount = material->frames.size();
  frames = new MeshMaterialFrame*[framesCount];
  for (u32 i = 0; i < framesCount; i++) {
    frames[i] = new MeshMaterialFrame(data, i, materialIndex);
  }

  _isMother = true;
}

MeshMaterial::MeshMaterial(const MeshBuilderData& data,
                           const u32& materialIndex)
    : color(false) {
  TYRA_ASSERT(materialIndex < data.materialsCount && materialIndex >= 0,
              "Provided index \"", materialIndex, "\" is out of range");

  id = rand() % 1000000;

  if (data.manyColorsEnabled) {
    singleColorFlag = false;
    TYRA_ASSERT(data.frames[0]->colors != nullptr, "Colors faces are required");
  } else {
    singleColorFlag = true;
  }

  color.set(128.0F, 128.0F, 128.0F, 128.0F);

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

  singleColorFlag = mesh.singleColorFlag;
  framesCount = mesh.framesCount;
  _name = mesh._name;

  color.set(128.0F, 128.0F, 128.0F, 128.0F);

  frames = new MeshMaterialFrame*[framesCount];
  for (u32 i = 0; i < framesCount; i++) {
    frames[i] = new MeshMaterialFrame(*mesh.frames[i]);
  }

  _isMother = false;
}

MeshMaterial::~MeshMaterial() {
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
      frames[0]->getColors() != nullptr,
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
  res << "Frames count: " << framesCount << ", " << std::endl;
  res << "Single color?: " << static_cast<u32>(singleColorFlag) << std::endl;
  res << ")";

  return res.str();
}

}  // namespace Tyra
