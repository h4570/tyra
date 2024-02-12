/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2024, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#pragma once

#include <tyra>
#include <memory>

namespace Tyra {

class Tutorial11 : public Game {
 public:
  explicit Tutorial11(Engine* engine);
  ~Tutorial11();

  void init();
  void loop();

 private:
  void setDrawData();
  void loadBags();

  void setTextureRegionRepeat();
  void setTextureDefaultWrap();

  const u8 MAX_TILES = 4;
  u8 TILE_COL = 0;
  u8 TILE_ROW = 0;

  u8 shouldFlipTextureSettings = false;
  float limitTimeToFlip = 2.00f;
  float elapsedTime = 0;

  Engine* engine;

  Vec4 cameraPosition, cameraLookAt;
  M4x4 translation, rotation, scale, model;
  Texture* UVCheckerTex;

  std::array<Vec4, 6> vertices;
  std::array<Color, 6> colors;
  std::array<Vec4, 6> uvMap;

  StaticPipeline stapip;
  std::unique_ptr<StaPipBag> bag;
  std::unique_ptr<StaPipInfoBag> infoBag;
  std::unique_ptr<StaPipColorBag> colorBag;
  std::unique_ptr<StaPipTextureBag> texBag;
};

}  // namespace Tyra
