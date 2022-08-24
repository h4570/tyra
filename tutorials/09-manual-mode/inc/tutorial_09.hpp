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
#include <memory>

namespace Tyra {

class Tutorial09 : public Game {
 public:
  Tutorial09(Engine* engine);
  ~Tutorial09();

  void init();
  void loop();

 private:
  void setDrawData();
  void loadBags();

  Engine* engine;

  StaticPipeline stapip;

  Vec4 cameraPosition, cameraLookAt;
  M4x4 translation, rotation, scale, model;

  std::array<Vec4, 3> vertices;
  std::array<Color, 3> colors;

  std::unique_ptr<StaPipBag> bag;
  std::unique_ptr<StaPipInfoBag> infoBag;
  std::unique_ptr<StaPipColorBag> colorBag;
};

}  // namespace Tyra
