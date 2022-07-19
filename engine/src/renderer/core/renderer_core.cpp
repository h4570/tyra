/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/renderer_core.hpp"
#include "thread/threading.hpp"

namespace Tyra {

RendererCore::RendererCore() { isFrameLimitOn = true; }
RendererCore::~RendererCore() {}

void RendererCore::init() {
  gs.init(&settings);
  texture.init(&gs, &path3);
  path3.init(&settings);
  renderer3D.init(&settings, &path1);
  renderer2D.init(&settings, &texture.clut);
  sync.init(&path3);
}

void RendererCore::setClearScreenColor(const Color& color) { bgColor = color; }

void RendererCore::beginFrame(const CameraInfo3D& cameraInfo) {
  renderer3D.update(cameraInfo);
  texture.onFrameChange();
  Threading::switchThread();
  path3.clearScreen(&gs.zBuffer, bgColor);
}

void RendererCore::endFrame() {
  Threading::switchThread();
  if (isFrameLimitOn) graph_wait_vsync();
  gs.flipBuffers();
}

}  // namespace Tyra
