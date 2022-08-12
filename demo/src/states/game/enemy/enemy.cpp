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
using Tyra::Math;
using Tyra::ObjLoader;
using Tyra::ObjLoaderOptions;

namespace Demo {

Enemy::Enemy(TextureRepository* repo) {
  ObjLoader loader;

  ObjLoaderOptions objOptions;
  objOptions.animation.count = 6;
  objOptions.flipUVs = true;
  objOptions.scale = 250.0F;

  auto* data = loader.load(FileUtils::fromCwd("game/models/zombie/zombie.obj"),
                           objOptions);
  data->normalsEnabled = false;
  mesh = new DynamicMesh(*data);

  delete data;

  mesh->playAnimation(0, mesh->frames.size() - 1);
  mesh->setAnimSpeed(0.1F);

  repo->addByMesh(mesh, FileUtils::fromCwd("game/models/zombie/"), "png");

  mesh->translation.translateY(-5.0F);

  allocateOptions();

  pair = new RendererDynamicPair{mesh, options};
}

Enemy::~Enemy() {
  delete mesh;
  delete options;
  delete pair;
}

void Enemy::update(const Heightmap& heightmap, const Vec4& playerPosition) {
  auto* enemyPosition = mesh->getPosition();

  auto diff = *enemyPosition - playerPosition;
  auto ang = Math::atan2(diff.x, diff.z);

  if (diff.length() > 30.0F) {
    diff.normalize();
    const float speed = 1.5F;
    auto nextPos = *enemyPosition - diff * speed;
    nextPos.y = heightmap.getHeightOffset(nextPos) - 60.0F;

    mesh->translation.identity();
    mesh->translation.translate(nextPos);
  }

  const float naturalRotation = 3.1F;
  mesh->rotation.identity();
  ang += naturalRotation;
  mesh->rotation.rotateByAngle(ang, Vec4(0.0F, 1.0F, 0.0F, 0.0F));

  mesh->animate();
}

void Enemy::allocateOptions() { options = new DynPipOptions(); }

}  // namespace Demo
