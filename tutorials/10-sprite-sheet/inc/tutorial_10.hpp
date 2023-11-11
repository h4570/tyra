/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022-2023, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Guido Diego Quispe Robles
*/

#pragma once

#include <tyra>
#include "font_sprite.hpp"

namespace Tyra {

class Tutorial10 : public Game {
 public:
  explicit Tutorial10(Engine* engine);
  ~Tutorial10();

  void init();
  void loop();

 private:
  void loadTexture();
  void loadSprite();
  void handlePad();

  int padTimer;
  Engine* engine;
  Pad* pad;
  Font font;

  Sprite sprite;
  Sprite spriteFlip;
  Sprite spriteScale;
  Sprite spriteStretch;
  std::string strFilter;

  PipelineTextureMappingType textureFilter;
};

}  // namespace Tyra
