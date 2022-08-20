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

#include <tamtypes.h>
#include "./core/renderer_core.hpp"
#include "./core/3d/camera_info_3d.hpp"
#include "./3d/renderer_3d.hpp"
#include "./2d/renderer_2d.hpp"

namespace Tyra {

class Renderer {
 public:
  Renderer();
  ~Renderer();

  Renderer2D renderer2D;
  Renderer3D renderer3D;

  RendererCore core;

  void init();

  /** World background color */
  void setClearScreenColor(const Color& color) {
    core.setClearScreenColor(color);
  }

  TextureRepository& getTextureRepository() { return core.texture.repository; }

  void setFrameLimit(const bool& onoff) { core.setFrameLimit(onoff); }

  /** Update view frustum for frustum culling. 3D support - off */
  void beginFrame();

  /** Update view frustum for frustum culling. 3D support - on */
  void beginFrame(const CameraInfo3D& cameraInfo);

  /** VSync and swap frame double buffer. */
  void endFrame();
};

}  // namespace Tyra
