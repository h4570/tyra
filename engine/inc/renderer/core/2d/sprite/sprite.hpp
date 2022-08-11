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
  Vec2 position, size;
  float scale;
  Color color;

  // ----
  // Getters
  // ----

  /** Get sprite drawing mode. */
  inline const SpriteMode& getMode() const { return mode; };

  // ----
  //  Setters
  // ----

  /** Set sprite drawing mode. */
  void setMode(const SpriteMode& t_val) { mode = t_val; }

  // ----
  //  Other
  // ----

  /** Flip texture horizontally. */
  inline void flipHorizontally(const u8& t_set) { _flipH = t_set; };

  /** Flip texture vertically. */
  inline void flipVertically(const u8& t_set) { _flipV = t_set; };

  /** Check if texture is vertically flipped. */
  inline const u8& isFlippedVertically() const { return _flipV; };

  /** Check if texture is horizontally flipped. */
  inline const u8& isFlippedHorizontally() const { return _flipH; };

 private:
  SpriteMode mode;
  u8 _isSizeSet, _flipH, _flipV;
  void setDefaultColor();
  void setDefaultLODAndClut();
};

}  // namespace Tyra
