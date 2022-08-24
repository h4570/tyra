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
#include "tutorial_09.hpp"

namespace Tyra {

Tutorial09::Tutorial09(Engine* t_engine) : engine(t_engine) {
  translation = M4x4::Identity;
  rotation = M4x4::Identity;
  scale = M4x4::Identity;
  model = M4x4::Identity;
}

Tutorial09::~Tutorial09() {}

void Tutorial09::init() {
  stapip.setRenderer(&engine->renderer.core);

  cameraPosition = Vec4(0.0F, 0.0F, -10.0F);
  cameraLookAt.unit();

  setDrawData();
  loadBags();
}

void Tutorial09::loop() {
  rotation.rotateY(0.1F);
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

void Tutorial09::setDrawData() {
  vertices = {
      Vec4(-3.0F, -3.0F, 0.0F),
      Vec4(3.0F, -3.0F, 0.0F),
      Vec4(0.0F, 3.0F, 0.0F),
  };

  colors = {
      Color(128.0F, 0.0F, 0.0F),
      Color(0.0F, 128.0F, 0.0F),
      Color(0.0F, 0.0F, 128.0F),
  };
}

void Tutorial09::loadBags() {
  /**
   * Create info bag and fill it with model matrix and shading type.
   * Keep rest of the options as default.
   */
  infoBag = std::make_unique<StaPipInfoBag>();
  infoBag->model = &model;
  infoBag->shadingType = TyraShadingGouraud;

  /** Create colors bag and set data pointer */
  colorBag = std::make_unique<StaPipColorBag>();
  colorBag->many = colors.begin();

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
  bag->info = infoBag.get();
  bag->vertices = vertices.begin();
  bag->count = vertices.size();
}

}  // namespace Tyra
