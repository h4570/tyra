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

namespace Tyra {

enum TextureBpp {

  /**
   * @brief 32-bit RGBA - Slowest
   */
  bpp32 = 32,

  /**
   * @brief 24-bit RGB - Slow
   */
  bpp24 = 24,

  /**
   * @brief 8-bit palletized (indexed) - Super fast
   */
  bpp8 = 8,

  /**
   * @brief 4-bit palletized (indexed) - Super, super fast
   */
  bpp4 = 4,
};

}  // namespace Tyra
