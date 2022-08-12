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

Weapon::Weapon(Engine* engine) {
  ObjLoader loader;

  ObjLoaderOptions objOptions;
  objOptions.flipUVs = true;
  objOptions.scale = .5F;

  pad = &engine->pad;
  audio = &engine->audio;
  auto* repo = &engine->renderer.core.texture.repository;
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

  initialPosition = *mesh->getPosition();

  allocateOptions();

  shootAdpcm =
      audio->adpcm.load(FileUtils::fromCwd("game/models/ak47/ak47.adpcm"));

  isShootAnimation1 = isShootAnimation2 = false;

  adpcmChannelsCount = 8;
  adpcmCurrentChannel = 0;

  const u8 shootVolume = 25;
  for (u8 i = 0; i < adpcmChannelsCount; i++) {
    audio->adpcm.setVolume(shootVolume, i);
  }
}

Weapon::~Weapon() {
  delete mesh;
  delete options;
  delete shootAdpcm;
}

u8 Weapon::getShootChannel() {
  auto result = adpcmCurrentChannel++;

  if (adpcmCurrentChannel >= adpcmChannelsCount) {
    adpcmCurrentChannel = 0;
  }
  return result;
}

void Weapon::update() {
  if (!isShootAnimation1 && !isShootAnimation2 && pad->getPressed().Cross) {
    shootTimer.prime();
    audio->adpcm.tryPlay(shootAdpcm, getShootChannel());
    isShootAnimation1 = true;
  }

  auto* position = mesh->getPosition();

  const float recoil = 0.5F;
  const float speed = 500;

  if (isShootAnimation1) {
    position->z += recoil;
    position->y -= recoil / 4;
    if (shootTimer.getTimeDelta() > speed) {
      isShootAnimation1 = false;
      shootTimer.prime();
      isShootAnimation2 = true;
    }
  }

  if (isShootAnimation2) {
    position->z -= recoil / 2;
    position->y += recoil / 8;
    if (shootTimer.getTimeDelta() > speed * 2) {
      isShootAnimation2 = false;
      *position = initialPosition;
      if (pad->getPressed().Cross) {
        shootTimer.prime();
        audio->adpcm.tryPlay(shootAdpcm, getShootChannel());
        isShootAnimation1 = true;
      }
    }
  }
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
