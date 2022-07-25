/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
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
  explicit Mesh(const MeshBuilderData& data);
  explicit Mesh(const Mesh& mesh);
  ~Mesh();

  /** Translation matrix */
  M4x4 translation;

  /** Rotation matrix */
  M4x4 rotation;

  /** Scale matrix */
  M4x4 scale;

  inline const u32& getId() const { return id; }
  inline const u8& isMother() const { return _isMother; }
  M4x4 getModelMatrix() const;

  /** Get position from translation matrix */
  inline Vec4* getPosition() {
    return reinterpret_cast<Vec4*>(&translation.data[3 * 4]);
  }

  void setPosition(const Vec4& v);

  /** Returns material, which is a mesh "subgroup". */
  MeshMaterial* getMaterial(const u32& i) const { return materials[i]; }

  MeshMaterial** getMaterials() { return materials; }

  const u32& getMaterialsCount() const { return materialsCount; }

 protected:
  void init();
  u8 _isMother;
  u32 id, materialsCount;
  MeshMaterial** materials;
};

}  // namespace Tyra
