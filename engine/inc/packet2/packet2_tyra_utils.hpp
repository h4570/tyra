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

#include <packet2_utils.h>
#include "math/m4x4.hpp"
#include "renderer/models/color.hpp"

namespace Tyra {

class Packet2TyraUtils {
 public:
  inline static void addM4x4(packet2_t* packet2, const M4x4& val) {
    *(reinterpret_cast<M4x4*>(packet2->next)) = val;
    packet2_advance_next(packet2, sizeof(MATRIX));
  }

  inline static void addVec4(packet2_t* packet2, const Vec4& val) {
    *(reinterpret_cast<Vec4*>(packet2->next)) = val;
    packet2_advance_next(packet2, sizeof(VECTOR));
  }

  inline static void addColor(packet2_t* packet2, const Color& val) {
    *(reinterpret_cast<Color*>(packet2->next)) = val;
    packet2_advance_next(packet2, sizeof(VECTOR));
  }
};

}  // namespace Tyra
