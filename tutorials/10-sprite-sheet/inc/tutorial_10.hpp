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
  FontData myFont;
  Color white;

  // sprites position
  const int normal = 0;   // 0
  const int flip = 1;     // 1
  const int scale = 2;    // 2
  const int stretch = 3;  // 3
  Sprite sprite[4];

  int posX1;
  int posX2;
  int posX3;
  int posX4;
  int posY1;
  int posY2;
  int posY3;
  int posY4;

  bool offsetIsNegative = false;
  std::string strFilter;

  PipelineTextureMappingType textureFilter;
};

}  // namespace Tyra
