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
#include "shared/renderer/core/texture/models/texture.hpp"

namespace Tyra {

class StaPipTextureBag {
 public:
  StaPipTextureBag();
  ~StaPipTextureBag();

  /** Mandatory. Texture coordinates per vertex. */
  Vec4* coordinates;

  /** Mandatory. Texture image. */
  Texture* texture;
};

}  // namespace Tyra
