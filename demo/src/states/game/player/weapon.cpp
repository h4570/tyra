/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/player/weapon.hpp"
#include <loaders/3d/obj_loader/obj_loader.hpp>
#include <file/file_utils.hpp>

using Tyra::FileUtils;
using Tyra::ObjLoader;
using Tyra::ObjLoaderOptions;

namespace Demo {

Weapon::Weapon(TextureRepository* repo) {
  ObjLoader loader;

  ObjLoaderOptions objOptions;
  objOptions.flipUVs = true;
  objOptions.scale = .5F;

  auto* data =
      loader.load(FileUtils::fromCwd("game/models/ak47/ak47.obj"), objOptions);
  data->normalsEnabled = false;
  mesh = new StaticMesh(*data);
  delete data;

  mesh->translation.translateX(2.85F);
  mesh->translation.translateY(-3.40F);
  mesh->translation.translateZ(-10.50F);

  mesh->rotation.rotateY(1.60F);
  mesh->rotation.rotateZ(-6.30F);

  mesh->scale.scale(0.5F);

  repo->addByMesh(mesh, FileUtils::fromCwd("game/models/ak47/"), "png");

  allocateOptions();
}

void Weapon::update() {}

// float tX = 0.0F;
// float tY = -3.0F;
// float tZ = -10.0F;

// float rY = 0.1F;
// float rZ = -4.3F;

// void Weapon::debugPosition(Pad* pad) {
//   const auto& leftJoy = pad->getLeftJoyPad();
//   const auto& rightJoy = pad->getRightJoyPad();
//   const float offset = 0.05F;

//   if (leftJoy.v <= 100) {
//     rY -= offset;
//   } else if (leftJoy.v >= 200) {
//     rY += offset;
//   }
//   if (leftJoy.h <= 100) {
//     rZ -= offset;
//   } else if (leftJoy.h >= 200) {
//     rZ += offset;
//   }

//   if (rightJoy.h <= 100) {
//     tY -= offset;
//   } else if (rightJoy.h >= 200) {
//     tY += offset;
//   }

//   if (rightJoy.v <= 100) {
//     tZ -= offset;
//   } else if (rightJoy.v >= 200) {
//     tZ += offset;
//   }

//   if (pad->getPressed().Circle) {
//     tX -= offset;
//   } else if (pad->getPressed().Cross) {
//     tX += offset;
//   }

//   mesh->rotation.identity();
//   mesh->rotation.rotateY(rY);
//   mesh->rotation.rotateZ(rZ);

//   mesh->translation.identity();
//   mesh->translation.translateX(tX);
//   mesh->translation.translateY(tY);
//   mesh->translation.translateZ(tZ);

//   TYRA_LOG("tX: ", tX, " tY: ", tY, " tZ: ", tZ, " rY: ", rY, " rZ: ", rZ);
// }

Weapon::~Weapon() {
  delete mesh;
  delete options;
}

void Weapon::allocateOptions() {
  options = new StaPipOptions();
  options->textureMappingType = Tyra::TyraNearest;
  options->shadingType = Tyra::TyraShadingFlat;
  options->blendingEnabled = false;
  options->fullClipChecks = false;
  options->frustumCulling = Tyra::PipelineFrustumCulling_None;
  options->transformationType = Tyra::TyraMP;
  options->antiAliasingEnabled = false;
}

}  // namespace Demo
