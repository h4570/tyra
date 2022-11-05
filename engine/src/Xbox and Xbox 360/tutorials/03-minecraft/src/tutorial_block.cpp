/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tyra>
#include "tutorial_block.hpp"

namespace Tyra {

TutorialBlock::TutorialBlock() {
  /** Set identity value for matrices (instead of random numbers) */
  translation = M4x4::Identity;
  rotation = M4x4::Identity;
  scale = M4x4::Identity;

  setRenderData();
}

/** Copy constructor */
TutorialBlock::TutorialBlock(const TutorialBlock& v) {
  translation = v.translation;
  rotation = v.rotation;
  scale = v.scale;
  model = v.model;
  color = v.color;
  atlasOffset = v.atlasOffset;

  setRenderData();
}

TutorialBlock::~TutorialBlock() {}

/** Fill block's render data object */
void TutorialBlock::setRenderData() {
  renderData.color = &color;
  renderData.model = &model;
  renderData.textureOffset = &atlasOffset;
}

void TutorialBlock::updateModelMatrix() {
  model = translation * rotation * scale;
}

}  // namespace Tyra
