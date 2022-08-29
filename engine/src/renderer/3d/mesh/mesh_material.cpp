
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tamtypes.h>
#include <string>
#include <cstdlib>
#include <iomanip>
#include "renderer/3d/mesh/mesh_material.hpp"
#include "file/file_utils.hpp"

namespace Tyra {

MeshMaterial::MeshMaterial(const MeshBuilderData& data,
                           const u32& materialIndex) {
  TYRA_ASSERT(materialIndex < data.materials.size(), "Provided index \"",
              materialIndex, "\" is out of range");

  auto* material = data.materials[materialIndex];

  TYRA_ASSERT(material->frames.size() > 0,
              "Material must have at least one frame");

  id = rand() % 1000000;

  if (data.loadLightmap) {
    lightmapFlag = true;
    TYRA_ASSERT(material->frames[0]->colors != nullptr,
                "Colors faces are required");
  } else {
    lightmapFlag = false;
  }

  ambient.set(material->ambient);
  name = material->name;

  if (material->texturePath.has_value()) {
    auto textureFilename =
        FileUtils::getFilenameFromPath(material->texturePath.value());

    textureName = FileUtils::getFilenameWithoutExtension(textureFilename);
  }

  TYRA_ASSERT(name.length() > 0, "MeshMaterial name cannot be empty");

  u32 lastVertexCount = 0;

  for (u32 i = 0; i < material->frames.size(); i++) {
    if (material->frames[i]->count > 0) {
      frames.push_back(new MeshMaterialFrame(data, i, materialIndex));

      if (i > 0) {
        TYRA_ASSERT(frames[i]->count == lastVertexCount,
                    "Vertex count must be the same for all frames");
      }

      lastVertexCount = frames[i]->count;
    } else {  // We will not use it, lets clean it up
      auto* frame = material->frames[i];
      if (frame->vertices != nullptr) delete[] frame->vertices;
      if (frame->normals != nullptr) delete[] frame->normals;
      if (frame->textureCoords != nullptr) delete[] frame->textureCoords;
      if (frame->colors != nullptr) delete[] frame->colors;
    }
  }

  isMother = true;
}

MeshMaterial::MeshMaterial(const MeshMaterial& mesh) {
  id = rand() % 1000000;

  lightmapFlag = mesh.lightmapFlag;
  name = mesh.name;
  textureName = mesh.textureName;
  ambient.set(128.0F, 128.0F, 128.0F, 128.0F);

  for (u32 i = 0; i < mesh.frames.size(); i++) {
    frames.push_back(new MeshMaterialFrame(*mesh.frames[i]));
  }

  isMother = false;
}

MeshMaterial::~MeshMaterial() {
  for (u32 i = 0; i < frames.size(); i++) {
    delete frames[i];
  }
}

const BBox& MeshMaterial::getBBox(const u32& frame) const {
  return *frames[frame]->bbox;
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
  res << "Name: " << this->name << ", " << std::endl;

  if (textureName.has_value()) {
    res << "Texture path: " << textureName.value() << ", " << std::endl;
  }

  res << "Frames count: " << frames.size() << ", " << std::endl;
  res << "Lightmap?: " << static_cast<u32>(lightmapFlag) << std::endl;
  res << ")";

  return res.str();
}

}  // namespace Tyra
