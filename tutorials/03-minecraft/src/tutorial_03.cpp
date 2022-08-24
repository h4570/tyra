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
#include "tutorial_03.hpp"

namespace Tyra {

Tutorial03::Tutorial03(Engine* t_engine) : engine(t_engine) {
  /** Lets set default value, to prevent nullptr bugs */
  textureAtlas = nullptr;
}

Tutorial03::~Tutorial03() {
  if (textureAtlas != nullptr) {
    engine->renderer.getTextureRepository().free(textureAtlas);
  }
}

void Tutorial03::init() {
  loadTexture();

  engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));

  cameraPosition = Vec4(0.0F, 0.0F, -10.0F);  // Move camera one step back
  cameraLookAt.unit();                        // Look at the origin - (0,0,0)

  /** Every 3D pipeline, needs to set RendererCore to it */
  mcpip.setRenderer(&engine->renderer.core);

  blocks.push_back(getBlock(0, 1, 2.0F));
  blocks.push_back(getBlock(4, 0, -2.0F));
}

void Tutorial03::loop() {
  rotateBlock(blocks[0]);
  rotateBlock(blocks[1]);

  /** Set camera info */
  engine->renderer.beginFrame(CameraInfo3D(&cameraPosition, &cameraLookAt));
  {
    /**
     * usePipeline() uploads pipeline's VU1 programs into VU1 memory and allows
     * us to use it.
     */
    engine->renderer.renderer3D.usePipeline(mcpip);
    {
      /** Render cobblestone */
      mcpip.render({&blocks[0].renderData}, textureAtlas);

      /** Render block using 6 textures from atlas (top-bottom) */
      mcpip.render({&blocks[1].renderData}, textureAtlas, true);
    }
  }
  engine->renderer.endFrame();
}

void Tutorial03::loadTexture() {
  auto& repo = engine->renderer.getTextureRepository();
  textureAtlas = repo.add(FileUtils::fromCwd("atlas.png"));
}

TutorialBlock Tutorial03::getBlock(const u8& atlasX, const u8& atlasY,
                                   const float& translationX) {
  TutorialBlock result;

  // --- Set texture atlas offset

  const u8 texturesInAtlasRow = 16;
  const float textureOffset = 1.0F / texturesInAtlasRow;

  /**
   * Offset is X/Y coordinate of left up texture corner.
   * Percentage is in range of 0.0F to 1.0F.
   */
  result.atlasOffset = Vec4(atlasX * textureOffset, atlasY * textureOffset);

  // --- Set default color

  result.color = Color(128.0F, 128.0F, 128.0F, 128.0F);

  result.translation.translateX(translationX);

  /** Build final matrix which will be used in renderer */
  result.updateModelMatrix();

  return result;
}

void Tutorial03::rotateBlock(TutorialBlock& block) {
  block.rotation.rotate(Vec4(0.01F, 0.02F, 0.03F));

  block.updateModelMatrix();
}

}  // namespace Tyra
