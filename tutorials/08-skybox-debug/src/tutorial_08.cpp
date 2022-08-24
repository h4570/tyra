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
#include "tutorial_08.hpp"

namespace Tyra {

Tutorial08::Tutorial08(Engine* t_engine) : engine(t_engine), fpsCounter(0) {}

Tutorial08::~Tutorial08() {
  engine->renderer.getTextureRepository().freeByMesh(skyboxMesh.get());
}

void Tutorial08::init() {
  stapip.setRenderer(&engine->renderer.core);

  cameraPosition = Vec4(0.0F, 0.0F, -10.0F);
  cameraLookAt.unit();

  loadSkyboxMesh();
  loadCupMesh();

  /** Uncomment this, to disable vsync */
  // engine->renderer.setFrameLimit(false);
}

void Tutorial08::loop() {
  if (fpsCounter++ > 50) {
    fpsCounter = 0;
    TYRA_LOG("FPS: ", engine->info.getFps(),
             " RAM: ", engine->info.getAvailableRAM());
  }

  cupMesh->rotation.rotate(Vec4(0.03F, 0.03F, 0.03F));

  auto& utilityTools = engine->renderer.renderer3D.utility;

  engine->renderer.beginFrame(CameraInfo3D(&cameraPosition, &cameraLookAt));
  {
    engine->renderer.renderer3D.usePipeline(stapip);
    stapip.render(skyboxMesh.get(), skyboxRenderOptions);
    stapip.render(cupMesh.get(), cupRenderOptions);

    /** Utility drawing should be done AFTER pipeline rendering */

    /** Draw cup bbox */
    utilityTools.drawBBox(
        cupMesh->frame->bbox->getTransformed(cupMesh->getModelMatrix()));

    /** Draw line */
    utilityTools.drawLine(Vec4(50.0F, 50.0F, 50.0F), *cupMesh->getPosition(),
                          Color(0.0F, 128.0F, 0.0F));

    /** Draw box at origin */
    utilityTools.drawBox(Vec4(0.0F, 0.0F, 0.0F), 0.2F,
                         Color(0.0F, 0.0F, 255.0F));
  }
  engine->renderer.endFrame();
}

void Tutorial08::loadSkyboxMesh() {
  ObjLoaderOptions options;
  options.scale = 200.0F;
  options.flipUVs = true;

  auto data = ObjLoader::load(FileUtils::fromCwd("skybox/skybox.obj"), options);

  skyboxMesh = std::make_unique<StaticMesh>(data.get());

  skyboxRenderOptions.frustumCulling = Tyra::PipelineFrustumCulling_None;

  /**
   * We need to put the skybox at infinite Z, so this options
   * will help us to do that.
   */
  skyboxRenderOptions.zTestType = Tyra::PipelineZTest_AllPass;

  engine->renderer.getTextureRepository().addByMesh(
      skyboxMesh.get(), FileUtils::fromCwd("skybox/"), "png");
}

void Tutorial08::loadCupMesh() {
  ObjLoaderOptions options;
  options.scale = 3.0F;

  auto data = ObjLoader::load(FileUtils::fromCwd("cup/cup.obj"), options);
  data->textureCoordsEnabled = false;

  cupMesh = std::make_unique<StaticMesh>(data.get());

  cupMesh->translation.translateZ(10.0F);
}

}  // namespace Tyra
