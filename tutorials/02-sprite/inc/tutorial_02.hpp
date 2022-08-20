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

#include <tyra>
#include <memory.h>

namespace Tyra {

class Tutorial02 : public Game {
 public:
  Tutorial02(Engine* engine);
  ~Tutorial02();

  void init();
  void loop();

 private:
  void loadTexture();
  void loadSprite();

  Engine* engine;

  Sprite sprite;
  Texture* texture;
};

}  // namespace Tyra
