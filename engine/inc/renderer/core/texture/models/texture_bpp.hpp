/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

namespace Tyra {

enum TextureBpp {

  // Tyra has 2.27MB~ free VRAM for textures.

  /**
   * @brief 32-bit RGBA - Slowest
   * Tyra can cache about 8~ of these textures (256x256)
   */
  bpp32 = 32,

  /**
   * @brief 24-bit RGB - Slow
   * Tyra can cache about 11~ of these textures (256x256)
   */
  bpp24 = 24,

  /**
   * @brief 8-bit palletized (indexed) - Super fast
   * Tyra can cache about 34~ of these textures (256x256)
   */
  bpp8 = 8,

  /**
   * @brief 4-bit palletized (indexed) - Super, super fast
   * Tyra can cache about 69~ of these textures (256x256)
   */
  bpp4 = 4,
};

}  // namespace Tyra
