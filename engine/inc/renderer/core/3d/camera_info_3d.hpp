/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "math/vec4.hpp"

namespace Tyra {

class CameraInfo3D {
 public:
  CameraInfo3D(Vec4* cameraPosition, Vec4* cameraLooksAt,
               Vec4* cameraUp = nullptr);
  ~CameraInfo3D();

  Vec4 *position, *looksAt, *up;

 private:
  static Vec4 defaultCameraUp;
};

}  // namespace Tyra
