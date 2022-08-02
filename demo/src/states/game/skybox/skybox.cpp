/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/skybox/skybox.hpp"
#include <loaders/3d/tyrobj/tyrobj_loader.hpp>
#include <file/file_utils.hpp>

using Tyra::FileUtils;
using Tyra::TyrobjLoader;

namespace Demo {

Skybox::Skybox(TextureRepository* repo) {
  TyrobjLoader loader;
  auto* data = loader.load(FileUtils::fromCwd("game/models/skybox/skybox.obj"),
                           1, 400.0F, true);
  data->normalsEnabled = false;
  mesh = new StaticMesh(*data);
  delete data;

  mesh->scale.scaleX(2.0F);
  mesh->scale.scaleZ(2.0F);

  for (u32 i = 0; i < mesh->getMaterialsCount(); i++) {
    auto* material = mesh->getMaterial(i);
    material->print();
    material->color.r = 16.0F;
    material->color.g = 16.0F;
    material->color.b = 16.0F;
  }

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
  options->textureMappingType = Tyra::TyraNearest;
  options->blendingEnabled = true;
  options->antiAliasingEnabled = false;
}

}  // namespace Demo
