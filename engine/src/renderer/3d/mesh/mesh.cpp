
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/mesh/mesh.hpp"

namespace Tyra {

Mesh::Mesh(const MeshBuilderData& data) {
  init();

  TYRA_ASSERT(data.materials.size() > 0,
              "Materials count must be greater than 0");

  for (u32 i = 0; i < data.materials.size(); i++) {
    auto* material = new MeshMaterial(data, i);

    if (material->frames.size() == 0) {
      TYRA_WARN("Found empty material: ", material->name, ". Skipping...");
      delete material;
      continue;
    }

    materials.push_back(material);
  }

  isMother = true;
}

Mesh::Mesh(const Mesh& mesh) {
  init();

  for (u32 i = 0; i < mesh.materials.size(); i++) {
    materials.push_back(new MeshMaterial(*mesh.materials[i]));
  }

  isMother = false;
}

M4x4 Mesh::getModelMatrix() const { return translation * rotation * scale; }

MeshMaterial* Mesh::getMaterialByName(const std::string& name) {
  for (auto* material : materials) {
    if (material->name == name) {
      return material;
    }
  }

  return nullptr;
}

Mesh::~Mesh() {
  for (u32 i = 0; i < materials.size(); i++) {
    delete materials[i];
  }
}

void Mesh::init() {
  id = rand() % 1000000;
  translation = M4x4::Identity;
  rotation = M4x4::Identity;
  scale = M4x4::Identity;
  translation.translate(Vec4(0.0F, 0.0F, 0.0F, 1.0F));
}

void Mesh::setPosition(const Vec4& v) {
  TYRA_ASSERT(v.w == 1.0F, "Vec4 must be homogeneous");
  reinterpret_cast<Vec4*>(&translation.data[3 * 4])->set(v);
}

}  // namespace Tyra
