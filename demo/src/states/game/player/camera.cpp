/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/player/camera.hpp"

namespace Demo {

Camera::Camera(Pad* t_pad) : lookAt(0.0F), position(0.0F), pad(t_pad) {
  lengthFromOrigin = 30.0F;
  circleRotation = -3.5F;
  height = 1.5F;
}

Camera::~Camera() {}

void Camera::update(const Vec4& playerPosition, const float& terrainHeight) {
  const float rotationOffset = 0.045F;
  const float heightOffset = 0.5F;

  const auto& rightJoy = pad->getRightJoyPad();

  if (rightJoy.h <= 100) {
    circleRotation += rotationOffset;
  } else if (rightJoy.h >= 200) {
    circleRotation -= rotationOffset;
  }

  if (rightJoy.v <= 100) {
    height += heightOffset;
  } else if (rightJoy.v >= 200) {
    height -= heightOffset;
  }

  unitCircle.x = (sin(circleRotation) * lengthFromOrigin);
  unitCircle.y = height;
  unitCircle.z = (cos(circleRotation) * lengthFromOrigin);

  position = playerPosition;
  lookAt = unitCircle + playerPosition;
}

}  // namespace Demo
