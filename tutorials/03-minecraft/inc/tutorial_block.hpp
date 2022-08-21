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

namespace Tyra {

class TutorialBlock {
 public:
  TutorialBlock();
  TutorialBlock(const TutorialBlock& v);
  ~TutorialBlock();

  void updateModelMatrix();

  M4x4 translation, rotation, scale;

  /** Result matrix with translation, rotation and scale  */
  M4x4 model;

  /** Color */
  Color color;

  /** Texture atlas offset */
  Vec4 atlasOffset;

  McpipBlock renderData;

 private:
  void setRenderData();
};

}  // namespace Tyra
