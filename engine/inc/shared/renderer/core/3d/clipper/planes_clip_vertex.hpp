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

#include "shared/math/vec4.hpp"

namespace Tyra {

struct PlanesClipVertex {
  Vec4 position, normal, st, color;
};

struct PlanesClipVertexPtrs {
  Vec4 *position, *normal, *st, *color;
};

}  // namespace Tyra
