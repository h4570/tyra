/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/skybox/skybox.hpp"

using Tyra::FileUtils;
using Tyra::ObjLoader;
using Tyra::ObjLoaderOptions;

namespace Demo {

Skybox::Skybox(TextureRepository* repo) {
  ObjLoader loader;

  ObjLoaderOptions objOptions;
  objOptions.flipUVs = true;
  objOptions.scale = 100.0F;

  auto data = loader.load(FileUtils::fromCwd("game/models/skybox/skybox.obj"),
                          objOptions);
  data->loadNormals = false;
  mesh = new StaticMesh(data.get());

  repo->addByMesh(mesh, FileUtils::fromCwd("game/models/skybox/"), "png");

  allocateOptions();

  pair = new RendererStaticPair{mesh, options};
}

Skybox::~Skybox() {
  delete mesh;
  delete options;
  delete pair;
}

void Skybox::update(const Vec4& playerPosition) {
  mesh->translation.identity();
  mesh->translation.translate(playerPosition);
}

void Skybox::allocateOptions() {
  options = new StaPipOptions();
  options->frustumCulling = Tyra::PipelineFrustumCulling_None;
  options->shadingType = Tyra::TyraShadingGouraud;
  options->zTestType = Tyra::PipelineZTest_AllPass;
  options->textureMappingType = Tyra::TyraNearest;
  options->blendingEnabled = true;
  options->antiAliasingEnabled = false;
}

}  // namespace Demo
