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
  objOptions.scale = 60.0F;

  auto* data = loader.load(
      FileUtils::fromCwd("game/models/soldier/soldier.obj"), objOptions);
  data->normalsEnabled = false;
  mesh = new DynamicMesh(*data);

  delete data;

  mesh->playAnimation(0, mesh->frames.size() - 1);
  mesh->setAnimSpeed(0.1F);

  repo->addByMesh(mesh, FileUtils::fromCwd("game/models/soldier/"), "png");

  mesh->translation.translateY(-5.0F);

  allocateOptions();

  pair = new RendererDynamicPair{mesh, options};

  direction = Vec4(1.0F, 0.0F, 1.0F, 0.0F);
}

Enemy::~Enemy() {
  delete mesh;
  delete options;
  delete pair;
}

void Enemy::update(const Heightmap& heightmap, const Vec4& playerPosition) {
  auto* pos = mesh->getPosition();
  auto nextPos = *pos + direction;

  if (heightmap.isOutside(nextPos)) {
    direction.x = -direction.x;
    direction.z = -direction.z;
  }

  nextPos = *pos + direction;
  nextPos.y = heightmap.getHeightOffset(nextPos) - 150.0F;
  mesh->translation.identity();
  mesh->translation.translate(nextPos);

  mesh->animate();
}

void Enemy::allocateOptions() { options = new DynPipOptions(); }

}  // namespace Demo
