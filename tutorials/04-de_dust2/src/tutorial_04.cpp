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
#include "tutorial_04.hpp"

namespace Tyra {

Tutorial04::Tutorial04(Engine* t_engine)
    : engine(t_engine), camera(&t_engine->pad) {}

Tutorial04::~Tutorial04() {
  engine->renderer.getTextureRepository().freeByMesh(mesh.get());
}

void Tutorial04::init() {
  stapip.setRenderer(&engine->renderer.core);
  loadMesh();
}

void Tutorial04::loop() {
  /** Update camera's lookAt and position */
  camera.update();

  engine->renderer.beginFrame(camera.getCameraInfo());
  {
    /** Upload static pipeline's VU1 programs */
    engine->renderer.renderer3D.usePipeline(stapip);

    /**
     * Render de_dust2.
     *
     * If you see clipping issues (visual glitches),
     * You can check how to fix them in #02 YT tutorial.
     */
    stapip.render(mesh.get(), renderOptions);
  }
  engine->renderer.endFrame();
}

void Tutorial04::loadMesh() {
  ObjLoaderOptions options;
  options.scale = 200.0F;

  /**
   * Load mesh data
   * Texture names are the material names "usemtl <material name>"
   */
  auto data =
      ObjLoader::load(FileUtils::fromCwd("de_dust2/de_dust2.obj"), options);

  /** Create mesh with loaded data */
  mesh = std::make_unique<StaticMesh>(data.get());

  /**
   * Disable frustum culling in render options,
   * because it not make sense to perform
   * frustum check on entire map (which will be always visible)
   */
  renderOptions.frustumCulling = Tyra::PipelineFrustumCulling_None;

  /**
   * Function which load all textures into texture repository,
   * based on material names.
   */
  engine->renderer.getTextureRepository().addByMesh(
      mesh.get(), FileUtils::fromCwd("de_dust2/"), "png");
}

}  // namespace Tyra
