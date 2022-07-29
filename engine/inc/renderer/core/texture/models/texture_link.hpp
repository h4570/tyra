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

#include <tamtypes.h>

namespace Tyra {
struct TextureLink {
  /**
   * For 3D: MeshMaterial material id.
   * For 2D: Sprite id.
   */
  u32 id;
};
}  // namespace Tyra
