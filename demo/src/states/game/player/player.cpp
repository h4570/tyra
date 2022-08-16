/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/player/player.hpp"

namespace Demo {

Player::Player(Engine* engine)
    : position(0.0F), weapon(engine), camera(&engine->pad) {
  pair = new RendererStaticPair{weapon.mesh, weapon.options};
  pad = &engine->pad;
  position = Vec4(3.0F, 50.0F, 0.0F);
  camera.lookAt = Vec4(0.0F, 0.0F, -30.0F);
  speed = 5.0F;

  utility = &engine->renderer.renderer3D.utility;
}

Player::~Player() { delete pair; }

void Player::update(const Heightmap& heightmap) {
  float terrainHeight = heightmap.getHeightOffset(getPosition());

  handlePlayerPosition(heightmap, terrainHeight);
  camera.update(position, terrainHeight);
  weapon.update();
}

PlayerShootAction Player::getShootAction() const {
  PlayerShootAction action;
  action.isShooting = weapon.isShooting;

  if (action.isShooting) {
    action.ray = Ray(camera.position, camera.lookAt.getNormalized());
  }

  utility->drawBox(camera.lookAt, 1.0F);

  // utility->drawLine(ray.origin, ray.direction);
  utility->drawLine(camera.position + Vec4(0.0F, 1.0F, 0.0F), camera.lookAt);

  return action;
}

void Player::handlePlayerPosition(const Heightmap& heightmap,
                                  const float& terrainHeight) {
  const auto& leftJoy = pad->getLeftJoyPad();

  auto normalizedCamera = Vec4(camera.unitCircle);
  normalizedCamera.normalize();
  normalizedCamera *= speed;

  Vec4 nextPosition(position);
  nextPosition.y = terrainHeight + 50.0F;

  if (leftJoy.v <= 100) {
    nextPosition.x += normalizedCamera.x;
    nextPosition.z += normalizedCamera.z;
  } else if (leftJoy.v >= 200) {
    nextPosition.x += -normalizedCamera.x;
    nextPosition.z += -normalizedCamera.z;
  }
  if (leftJoy.h <= 100) {
    nextPosition.x += normalizedCamera.z;
    nextPosition.z += -normalizedCamera.x;
  } else if (leftJoy.h >= 200) {
    nextPosition.x += -normalizedCamera.z;
    nextPosition.z += normalizedCamera.x;
  }

  if (!heightmap.isOutside(nextPosition)) position = nextPosition;

  camera.position = position;
}

}  // namespace Demo
