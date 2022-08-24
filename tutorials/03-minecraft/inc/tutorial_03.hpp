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
#include <vector>
#include "./tutorial_block.hpp"

namespace Tyra {

class Tutorial03 : public Game {
 public:
  Tutorial03(Engine* engine);
  ~Tutorial03();

  void init();
  void loop();

 private:
  void loadTexture();

  TutorialBlock getBlock(const u8& atlasX, const u8& atlasY,
                         const float& translationX);
  void rotateBlock(TutorialBlock& block);

  Engine* engine;

  std::vector<TutorialBlock> blocks;

  Texture* textureAtlas;

  /**
   * This is a 3D pipeline (renderer) which we will be using.
   *
   * Tyra has 3 pipelines:
   * - Minecraft (this one)
   * - Static (for static 3D objects)
   * - Dynamic (for dynamic 3D objects)
   *
   * You can create your own VU1 programs and create your custom pipelines!
   */
  MinecraftPipeline mcpip;

  /** Our camera */
  Vec4 cameraPosition, cameraLookAt;
};

}  // namespace Tyra
