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
#include <functional>

using Tyra::Math;

namespace Demo {

Enemy::Enemy(Engine* engine, const EnemyInfo& t_info) {
  info = t_info;
  utility = &engine->renderer.renderer3D.utility;

  mesh = new DynamicMesh(*info.motherMesh);

  auto* bodyMaterial = mesh->getMaterialByName("BodyMaterial");
  auto* clothMaterial = mesh->getMaterialByName("ClothMaterial");

  TYRA_ASSERT(bodyMaterial != nullptr && clothMaterial != nullptr,
              "Body and cloth materials not found!");

  info.bodyTexture->addLink(bodyMaterial->id);
  info.clothTexture->addLink(clothMaterial->id);

  float r = Math::randomf(64.0F, 128.0F);
  float g = Math::randomf(64.0F, 128.0F);
  float b = Math::randomf(64.0F, 128.0F);

  bodyMaterial->ambient.set(r, g, b);
  clothMaterial->ambient.set(r, g, b);

  walkSequence = {0, 1, 2};
  fightSequence = {3, 4, 5};

  spawnPoint = info.spawnPoint;
  mesh->setPosition(info.spawnPoint);

  mesh->animation.setSequence(walkSequence);
  mesh->animation.loop = true;
  mesh->animation.setCallback(
      std::bind(&Enemy::animationCallback, this, std::placeholders::_1));
  mesh->translation.translateY(-5.0F);

  audio = &engine->audio;
  audio->adpcm.setVolume(30, info.adpcmChannel);

  allocateOptions();

  pair = new RendererDynamicPair{mesh, options};
}

Enemy::~Enemy() {
  delete mesh;
  delete options;
  delete pair;
}

void Enemy::update(const Heightmap& heightmap, const Vec4& playerPosition,
                   const PlayerShootAction& shootAction) {
  auto* enemyPosition = mesh->getPosition();

  handlePlayerShoot(shootAction);

  auto diff = *enemyPosition - playerPosition;
  auto ang = Math::atan2(diff.x, diff.z);

  if (diff.length() > 110.0F) {
    walk(heightmap, diff);
  } else {
    fight();
  }

  const float naturalRotation = 3.1F;
  mesh->rotation.identity();
  ang += naturalRotation;
  mesh->rotation.rotateByAngle(ang, Vec4(0.0F, 1.0F, 0.0F, 0.0F));

  mesh->update();
}

void Enemy::handlePlayerShoot(const PlayerShootAction& shootAction) {
  if (!shootAction.isShooting) {
    return;
  }

  const auto& ray = shootAction.ray.value();
  auto bbox =
      mesh->getCurrentBoundingBox().getTransformed(mesh->getModelMatrix());

  auto isOnEnemy = ray.intersectBox(bbox.min(), bbox.max());

  utility->drawBBox(bbox);
  utility->drawLine(ray.origin, ray.direction);
  utility->drawPoint(*mesh->getPosition());

  if (isOnEnemy) {
    mesh->setPosition(spawnPoint);
    TYRA_LOG("Enemy hit!");
  }
}

void Enemy::walk(const Heightmap& heightmap, const Vec4& positionDiff) {
  if (isFighting) {
    mesh->animation.setSequence(walkSequence);
  }

  isFighting = false;
  isWalking = true;

  auto* enemyPosition = mesh->getPosition();
  auto normalized = positionDiff;
  normalized.normalize();
  const float speed = 2.5F;
  auto nextPos = *enemyPosition - normalized * speed;
  nextPos.y = heightmap.getHeightOffset(nextPos) - 60.0F;

  mesh->translation.identity();
  mesh->translation.translate(nextPos);
}

void Enemy::fight() {
  if (isWalking) {
    mesh->animation.setSequence(fightSequence);
  }

  isFighting = true;
  isWalking = false;
}

void Enemy::animationCallback(const AnimationSequenceCallback& callback) {
  if (callback == AnimationSequenceCallback::AnimationSequenceCallback_Loop) {
    if (isFighting) {
      audio->adpcm.tryPlay(info.adpcmSample, info.adpcmChannel);
    }
  }
}

void Enemy::allocateOptions() { options = new DynPipOptions(); }

}  // namespace Demo
