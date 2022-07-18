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
#include <tamtypes.h>

namespace Tyra {

class Mesh {
 public:
  Mesh(const u8& isMother);
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

  inline void setPosition(const Vec4& v) {
    TYRA_ASSERT(v.w == 1.0F, "Vec4 must be homogeneous");
    reinterpret_cast<Vec4*>(&translation.data[3 * 4])->set(v);
  }

 protected:
  u8 _isMother;
  u32 id;
};

}  // namespace Tyra
