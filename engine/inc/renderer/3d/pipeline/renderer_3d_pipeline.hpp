/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "renderer/core/renderer_core.hpp"

namespace Tyra {

class Renderer3DPipeline {
 public:
  Renderer3DPipeline() {}
  ~Renderer3DPipeline() {}

  virtual void setRenderer(RendererCore* core) = 0;
  virtual void onUse() = 0;
  virtual void onUseEnd() = 0;
};

}  // namespace Tyra
