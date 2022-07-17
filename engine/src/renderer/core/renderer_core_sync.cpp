/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/renderer_core_sync.hpp"

namespace Tyra {

RendererCoreSync::RendererCoreSync() {}
RendererCoreSync::~RendererCoreSync() {}

void RendererCoreSync::init(Path3* t_path3) { path3 = t_path3; }

void RendererCoreSync::align() {
  clear();
  add();
  waitAndClear();
}

void RendererCoreSync::add() { path3->addDrawFinishTag(); }

u8 RendererCoreSync::check() { return *GS_REG_CSR & 2; }

void RendererCoreSync::clear() { *GS_REG_CSR |= 2; }

void RendererCoreSync::waitAndClear() {
  while (!check()) {
  }
  clear();
}

}  // namespace Tyra
