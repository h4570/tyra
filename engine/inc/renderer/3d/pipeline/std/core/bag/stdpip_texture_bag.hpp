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

#include "math/vec4.hpp"
#include "renderer/core/texture/models/texture.hpp"

namespace Tyra {

class StdpipTextureBag {
 public:
  StdpipTextureBag();
  ~StdpipTextureBag();

  /** Mandatory. Texture coordinates per vertex. */
  Vec4* coordinates;

  /** Mandatory. Texture image. */
  Texture* texture;
};

}  // namespace Tyra
