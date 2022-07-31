/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/terrain/terrain.hpp"
#include <loaders/3d/tyrobj/tyrobj_loader.hpp>
#include <file/file_utils.hpp>

using Tyra::FileUtils;
using Tyra::TyrobjLoader;

namespace Demo {

Terrain::Terrain(TextureRepository* repo) {
  TyrobjLoader loader;
  auto* data = loader.load(
      FileUtils::fromCwd("game/models/terrain/terrain.obj"), 1, 25.0F, true);
  data->normalsEnabled = false;
  mesh = new StaticMesh(*data);
  mesh->getMaterial(0)->color.r = 96.0F;
  mesh->getMaterial(0)->color.g = 96.0F;
  mesh->getMaterial(0)->color.b = 96.0F;
  delete data;

  repo->addByMesh(mesh, FileUtils::fromCwd("game/models/terrain/"), "png");

  allocateOptions();

  pair = new RendererStaticPair{mesh, options};
}

// float testRotation = 0.0F;

void Terrain::update() {
  // testRotation += 0.001F;
  mesh->rotation.identity();
  mesh->rotation.rotateZ(0.1F);
  mesh->rotation.rotateY(-4.3F);
  // TYRA_LOG(testRotation);
}

Terrain::~Terrain() {
  delete mesh;
  delete options;
  delete pair;
}

void Terrain::allocateOptions() {
  options = new StaPipOptions();
  options->shadingType = Tyra::TyraShadingGouraud;
  options->blendingEnabled = true;
  options->antiAliasingEnabled = false;
}

}  // namespace Demo
