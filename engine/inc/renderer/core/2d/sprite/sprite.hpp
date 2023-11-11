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

#include <draw_types.h>
#include <draw_buffers.h>
#include "./sprite_mode.hpp"
#include "math/vec2.hpp"
#include "renderer/models/color.hpp"

namespace Tyra {

class Sprite {
 public:
  Sprite();
  ~Sprite();

  u32 id;
  Vec2 position, size, offset;
  float scale;
  Color color;
  SpriteMode mode;
  bool flipHorizontal, flipVertical;

 private:
  void setDefaultColor();
};

}  // namespace Tyra
