/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "math/vec4.hpp"
#include "renderer/core/3d/camera_info_3d.hpp"
#include "pad/pad.hpp"

using Tyra::CameraInfo3D;
using Tyra::Pad;
using Tyra::Vec4;

namespace Demo {

class Camera {
 public:
  Camera(Pad* pad);
  ~Camera();

  Vec4 lookAt;
  Vec4 position;
  Vec4 unitCircle;

  CameraInfo3D getCameraInfo() { return CameraInfo3D(&position, &lookAt); }

  void update(const Vec4& playerPosition, const float& terrainHeight);

 private:
  float circleRotation, lengthFromOrigin, height;

  Pad* pad;
};

}  // namespace Demo
