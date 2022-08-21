/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "math/m4x4.hpp"
#include "./mesh_material.hpp"
#include <tamtypes.h>
#include "debug/debug.hpp"

namespace Tyra {

class Mesh {
 public:
  explicit Mesh(const MeshBuilderData* data);
  Mesh(const Mesh& mesh);
  ~Mesh();

  u8 isMother;

  u32 id;

  M4x4 translation, rotation, scale;

  /** nullptr if not found */
  MeshMaterial* getMaterialByName(const std::string& name);

  std::vector<MeshMaterial*> materials;

  M4x4 getModelMatrix() const;

  /** Get position from translation matrix */
  inline Vec4* getPosition() {
    return reinterpret_cast<Vec4*>(&translation.data[3 * 4]);
  }

  void setPosition(const Vec4& v);

 protected:
  void init();
};

}  // namespace Tyra
