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

#include <tyra>

namespace Tyra {

class Camera {
 public:
  Camera(Pad* pad);
  ~Camera();

  Vec4 lookAt;
  Vec4 position;

  /**
   * Result variable of unit circle calculation
   * This is needed for rotation about the camera lookAt
   * https://www.geogebra.org/m/cNEtsbvC
   */
  Vec4 unitCircle;

  CameraInfo3D getCameraInfo() { return CameraInfo3D(&position, &lookAt); }

  void update();

 private:
  void updatePosition();
  void updateLookAt();

  Pad* pad;
  float circleRotation, circleLength, speed, lookAtHeight;
};

}  // namespace Tyra
