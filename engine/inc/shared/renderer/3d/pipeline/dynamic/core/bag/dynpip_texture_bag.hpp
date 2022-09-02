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

class DynPipTextureBag {
 public:
  DynPipTextureBag();
  ~DynPipTextureBag();

  /** Mandatory. Texture coordinates per vertex. */
  Vec4* coordinatesFrom;
  Vec4* coordinatesTo;

  /** Mandatory. Texture image. */
  Texture* texture;

  void freeCoords();
};

}  // namespace Tyra
