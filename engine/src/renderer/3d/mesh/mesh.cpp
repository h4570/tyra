
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/mesh/mesh.hpp"

namespace Tyra {

Mesh::Mesh(const MeshBuilderData& data) {
  init();

  TYRA_ASSERT(data.materials.size() > 0,
              "Materials count must be greater than 0");

  materialsCount = data.materials.size();
  materials = new MeshMaterial*[materialsCount];

  for (u32 i = 0; i < materialsCount; i++) {
    materials[i] = new MeshMaterial(data, i);
  }

  _isMother = true;
}

Mesh::Mesh(const Mesh& mesh) {
  init();

  materialsCount = mesh.materialsCount;
  materials = new MeshMaterial*[materialsCount];

  for (u32 i = 0; i < materialsCount; i++) {
    materials[i] = new MeshMaterial(*mesh.materials[i]);
  }

  _isMother = false;
}

M4x4 Mesh::getModelMatrix() const { return translation * rotation * scale; }

Mesh::~Mesh() {
  for (u32 i = 0; i < materialsCount; i++) {
    delete materials[i];
  }
  delete[] materials;
}

void Mesh::init() {
  id = rand() % 1000000;
  materialsCount = 0;
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
