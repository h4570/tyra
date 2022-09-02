/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/renderer/core/renderer_core_sync.hpp"

namespace Tyra {

RendererCoreSync::RendererCoreSync() {}
RendererCoreSync::~RendererCoreSync() {}

void RendererCoreSync::init(Path3* t_path3, Path1* t_path1) {
  path3 = t_path3;
  path1 = t_path1;
}

void RendererCoreSync::align3D() {
  clear();
  sendPath1Req();
  waitAndClear();
}

void RendererCoreSync::align2D() {
  clear();
  sendPath3Req();
  waitAndClear();
}

void RendererCoreSync::sendPath1Req() { path1->sendDrawFinishTag(); }

void RendererCoreSync::sendPath3Req() { path3->sendDrawFinishTag(); }

void RendererCoreSync::addPath1Req(packet2_t* packet) {
  path1->addDrawFinishTag(packet);
}

unsigned char RendererCoreSync::check() { return *GS_REG_CSR & 2; }

void RendererCoreSync::clear() { *GS_REG_CSR |= 2; }

void RendererCoreSync::waitAndClear() {
  while (!check()) {
  }
  clear();
}

}  // namespace Tyra
