/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2024, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include <tyra>
#include "tutorial_11.hpp"

namespace Tyra {

Tutorial11::Tutorial11(Engine* t_engine) : engine(t_engine) {
  translation = M4x4::Identity;
  rotation = M4x4::Identity;
  scale = M4x4::Identity;
  model = M4x4::Identity;

  rotation.rotateY(Math::PI);
}

Tutorial11::~Tutorial11() {}

void Tutorial11::init() {
  stapip.setRenderer(&engine->renderer.core);

  cameraPosition = Vec4(0.0F, 0.0F, -10.0F);
  cameraLookAt.unit();

  UVCheckerTex = engine->renderer.getTextureRepository().add(
      FileUtils::fromCwd("uv_checker.png"));

  setDrawData();
  loadBags();
}

void Tutorial11::loop() {
  const auto deltaTime =
      std::min(1.0f / static_cast<float>(engine->info.getFps()),
               static_cast<float>(1.0 / 60.0f));
  elapsedTime += deltaTime;

  if (elapsedTime > limitTimeToFlip) {
    elapsedTime -= limitTimeToFlip;
    shouldFlipTextureSettings = !shouldFlipTextureSettings;
    shouldFlipTextureSettings ? setTextureRegionRepeat()
                              : setTextureDefaultWrap();

    // Update texture packet info after change wrap settings
    engine->renderer.core.texture.updateTextureInfo(UVCheckerTex);
  }

  model = translation * rotation * scale;

  engine->renderer.beginFrame(CameraInfo3D(&cameraPosition, &cameraLookAt));
  {
    engine->renderer.renderer3D.usePipeline(stapip);

    /**
     * Lets draw manually via "bags".
     * You can do the same thing with dynamic pipeline.
     */
    stapip.core.render(bag.get());
  }
  engine->renderer.endFrame();
}

void Tutorial11::setDrawData() {
  vertices = {
      // Tri 1
      Vec4(-3.0F, -3.0F, 0.0F),
      Vec4(3.0F, -3.0F, 0.0F),
      Vec4(-3.0F, 3.0F, 0.0F),

      // Tri 2
      Vec4(3.0F, -3.0F, 0.0F),
      Vec4(-3.0F, 3.0F, 0.0F),
      Vec4(3.0F, 3.0F, 0.0F),
  };

  colors = {
      Color(128.0F, 128.0F, 128.0F), Color(128.0F, 128.0F, 128.0F),
      Color(128.0F, 128.0F, 128.0F), Color(128.0F, 128.0F, 128.0F),
      Color(128.0F, 128.0F, 128.0F), Color(128.0F, 128.0F, 128.0F),
  };

  setTextureDefaultWrap();
}

void Tutorial11::setTextureDefaultWrap() {
  uvMap = {Vec4(0.0f, 1.0F, 1.0F, 0.0f),       Vec4(0.0f + 1, 1.0F, 1.0F, 0.0f),
           Vec4(0.0f, 0.0f, 1.0F, 0.0f),

           Vec4(0.0f + 1, 1.0F, 1.0F, 0.0f),   Vec4(0.0f, 0.0f, 1.0F, 0.0f),
           Vec4(0.0f + 1.0F, 0.0f, 1.0F, 0.0f)};

  TYRA_LOG("Reseting texture wrap settings");
  UVCheckerTex->setDefaultWrapSettings();
}

void Tutorial11::setTextureRegionRepeat() {
  const float _scale = 1.0f / (float)MAX_TILES;
  const Vec4 scale =
      Vec4(MAX_TILES > 0 ? _scale * MAX_TILES : _scale,
           MAX_TILES > 0 ? _scale * MAX_TILES : _scale, 1.0f, 0.0f);

  uvMap = {
      Vec4(TILE_COL, TILE_ROW + 1.0F, 1.0F, 0.0f) * scale,
      Vec4(TILE_COL + 1, TILE_ROW + 1.0F, 1.0F, 0.0f) * scale,
      Vec4(TILE_COL, TILE_ROW, 1.0F, 0.0f) * scale,

      Vec4(TILE_COL + 1, TILE_ROW + 1.0F, 1.0F, 0.0f) * scale,
      Vec4(TILE_COL, TILE_ROW, 1.0F, 0.0f) * scale,
      Vec4(TILE_COL + 1.0F, TILE_ROW, 1.0F, 0.0f) * scale,
  };

  const auto tileW = (UVCheckerTex->getWidth() * _scale) - 1;
  const auto tileH = (UVCheckerTex->getHeight() * _scale) - 1;
  const auto tileU = TILE_COL * tileW + TILE_COL;
  const auto tileV = TILE_ROW * tileH + TILE_ROW;

  int minu = tileW;
  int minv = tileH;
  int maxu = tileU;
  int maxv = tileV;

  TYRA_LOG("Changing the texture wraping...");
  UVCheckerTex->setWrapSettings(TextureWrap::RegionRepeat,
                                TextureWrap::RegionRepeat, minu, minv, maxu,
                                maxv);

  TILE_COL++;
  if (TILE_COL >= MAX_TILES) {
    TILE_COL %= MAX_TILES;
    TILE_ROW = (TILE_ROW + 1) % MAX_TILES;
  }
}

void Tutorial11::loadBags() {
  /**
   * Create info bag and fill it with model matrix and shading type.
   * Keep rest of the options as default.
   */
  infoBag = std::make_unique<StaPipInfoBag>();
  infoBag->model = &model;
  infoBag->shadingType = TyraShadingGouraud;
  infoBag->textureMappingType = TyraNearest;

  /** Create colors bag and set data pointer */
  colorBag = std::make_unique<StaPipColorBag>();
  colorBag->many = colors.begin();

  texBag = std::make_unique<StaPipTextureBag>();
  texBag.get()->texture = UVCheckerTex;
  texBag.get()->coordinates = uvMap.data();

  /**
   * Create main bag and set:
   * - vertices data
   * - vertices count
   * - pointers to other bags
   *
   * You can add here lighting and texture bag as well.
   * If you want to have deeper look of this feature, please
   * read static_pipeline.cpp code file.
   */
  bag = std::make_unique<StaPipBag>();
  bag->color = colorBag.get();
  bag->texture = texBag.get();
  bag->info = infoBag.get();
  bag->vertices = vertices.begin();
  bag->count = vertices.size();
}

}  // namespace Tyra
