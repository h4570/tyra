/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/renderer.hpp"

namespace Tyra {

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::init() {
  core.init();
  renderer2D.init(&core);
}

void Renderer::beginFrame(const CameraInfo3D& cameraInfo) {
  core.beginFrame(cameraInfo);
}

void Renderer::endFrame() { core.endFrame(); }

}  // namespace Tyra
