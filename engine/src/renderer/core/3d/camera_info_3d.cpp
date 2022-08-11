/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/3d/camera_info_3d.hpp"

namespace Tyra {

Vec4 CameraInfo3D::defaultCameraUp = Vec4(0.0F, 1.0F, 0.0F);

CameraInfo3D::CameraInfo3D(Vec4* t_cameraPosition, Vec4* t_cameraLooksAt,
                           Vec4* t_cameraUp) {
  position = t_cameraPosition;
  looksAt = t_cameraLooksAt;

  if (t_cameraUp == nullptr) {
    up = &defaultCameraUp;
  } else {
    up = t_cameraUp;
  }
}
CameraInfo3D::~CameraInfo3D() {}

}  // namespace Tyra
