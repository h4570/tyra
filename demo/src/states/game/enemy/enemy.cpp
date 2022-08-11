/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/enemy/enemy.hpp"
#include "loaders/3d/obj_loader/obj_loader.hpp"
#include <file/file_utils.hpp>

using Tyra::FileUtils;
using Tyra::ObjLoader;
using Tyra::ObjLoaderOptions;

namespace Demo {

Enemy::Enemy(TextureRepository* repo) {
  ObjLoader loader;

  ObjLoaderOptions objOptions;
  objOptions.animation.count = 4;
  objOptions.flipUVs = true;
  objOptions.scale = 25.0F;

  auto* bodyData = loader.load(
      FileUtils::fromCwd("game/models/soldier/soldier.obj"), objOptions);
  bodyData->normalsEnabled = false;
  bodyMesh = new DynamicMesh(*bodyData);

  delete bodyData;

  bodyMesh->playAnimation(0, bodyMesh->frames.size() - 1);
  bodyMesh->setAnimSpeed(0.1F);

  repo->addByMesh(bodyMesh, FileUtils::fromCwd("game/models/soldier/"), "png");

  bodyMesh->translation.translateY(-5.0F);

  allocateOptions();

  pairs.push_back(new RendererDynamicPair{bodyMesh, options});
  // pairs.push_back(new RendererDynamicPair{gunMesh, options});
}

Enemy::~Enemy() {
  delete bodyMesh;
  // delete gunMesh;
  delete options;
  for (auto* pair : pairs) {
    delete pair;
  }
}

void Enemy::update(const Vec4& playerPosition) { bodyMesh->animate(); }

void Enemy::allocateOptions() { options = new DynPipOptions(); }

}  // namespace Demo
