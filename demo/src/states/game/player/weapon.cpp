/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/player/weapon.hpp"
#include <loaders/3d/tyrobj/tyrobj_loader.hpp>
#include <file/file_utils.hpp>

using Tyra::FileUtils;
using Tyra::TyrobjLoader;

namespace Demo {

Weapon::Weapon(TextureRepository* repo) {
  TyrobjLoader loader;
  auto* data = loader.load(FileUtils::fromCwd("game/models/ak47/ak47.obj"), 1,
                           1.0F, true);
  data->normalsEnabled = false;
  mesh = new StaticMesh(*data);
  delete data;

  mesh->translation.translateZ(-50.0F);

  repo->addByMesh(mesh, FileUtils::fromCwd("game/models/ak47/"), "png");

  allocateOptions();
}

// float testRotation = 0.0F;

void Weapon::update() {
  // testRotation += 0.001F;
  mesh->rotation.identity();
  mesh->rotation.rotateZ(0.1F);
  mesh->rotation.rotateY(-4.3F);
  // TYRA_LOG(testRotation);
}

Weapon::~Weapon() {
  delete mesh;
  delete options;
}

void Weapon::allocateOptions() {
  options = new StaPipOptions();
  options->shadingType = Tyra::TyraShadingGouraud;
  options->blendingEnabled = false;
  options->antiAliasingEnabled = false;
}

}  // namespace Demo
