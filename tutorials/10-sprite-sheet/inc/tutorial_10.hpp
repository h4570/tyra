/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022-2023, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Guido Diego Quispe Robles
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <tyra>
#include "fontSprite.hpp"

namespace Tyra {

class Tutorial10 : public Game {
 public:
  Tutorial10(Engine* engine);
  ~Tutorial10();

  void init();
  void loop();

 private:
  void loadTexture();
  void loadSprite();

  Engine* engine;
  Pad* pad;
  Font font;

  Sprite sprite;
  Sprite spriteFlip;
  Sprite spriteScale;
  Sprite spriteStrech;
  std::string str_filter;

  PipelineTextureMappingType textureFilter;
};

}  // namespace Tyra
