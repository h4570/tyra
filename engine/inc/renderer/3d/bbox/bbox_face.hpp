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

#include "math/vec4.hpp"

namespace Tyra {

/** BBox face */
class BBoxFace {
 public:
  BBoxFace() {}
  BBoxFace(const Vec4& t_minCorner, const Vec4& t_maxCorner, float t_axisPos) {
    minCorner = t_minCorner;
    maxCorner = t_maxCorner;
    axisPosition = t_axisPos;
  }

  /** Position of the face on it's respective axis. */
  float axisPosition;
  /** Corner of the face with lower coordinates */
  Vec4 minCorner;
  /** Corner of the face with higher coordinates */
  Vec4 maxCorner;
};

}  // namespace Tyra
