
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/renderer_3d.hpp"

namespace Tyra {

Renderer3D::Renderer3D() { currentPipeline = nullptr; }

Renderer3D::~Renderer3D() {}

void Renderer3D::init(RendererCore* core) { utility.init(core); }

void Renderer3D::onFrameEnd() {
  if (currentPipeline != nullptr) {
    currentPipeline->onFrameEnd();
  }
}

void Renderer3D::usePipeline(Renderer3DPipeline& pipeline) {
  usePipeline(&pipeline);
}

void Renderer3D::usePipeline(Renderer3DPipeline* pipeline) {
  if (currentPipeline != pipeline) {
    if (currentPipeline) currentPipeline->onUseEnd();
    currentPipeline = pipeline;
    currentPipeline->onDestroy =
        std::bind(&Renderer3D::onDestroy, this, std::placeholders::_1);
    currentPipeline->onUse();
  }
}

void Renderer3D::onDestroy(Renderer3DPipeline* pipeline) {
  if (currentPipeline == pipeline) {
    currentPipeline->onUseEnd();
    currentPipeline = nullptr;
  }
}

}  // namespace Tyra
