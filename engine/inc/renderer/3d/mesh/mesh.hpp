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
  explicit Mesh();
  ~Mesh();

  /** Translation matrix */
  M4x4 translation;

  /** Rotation matrix */
  M4x4 rotation;

  /** Scale matrix */
  M4x4 scale;

  const u32& getId() const { return id; }

 protected:
  u32 id;
};

}  // namespace Tyra
