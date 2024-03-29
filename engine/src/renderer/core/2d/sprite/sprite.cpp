/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/2d/sprite/sprite.hpp"

namespace Tyra {

Sprite::Sprite() {
  id = rand() % 1000000;
  flipHorizontal = false;
  flipVertical = false;
  size.set(32.0F, 32.0F);
  position.set(100.0F, 100.0F);
  offset.set(0.0F, 0.0F);
  scale = 1.0F;
  mode = MODE_REPEAT;
  setDefaultColor();
}

Sprite::~Sprite() {}

// ----
// Methods
// ----

void Sprite::setDefaultColor() {
  color.r = 128;
  color.g = 128;
  color.b = 128;
  color.a = 128;
}

}  // namespace Tyra