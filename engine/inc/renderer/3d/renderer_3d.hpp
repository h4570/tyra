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

#include "./pipeline/renderer_3d_pipeline.hpp"

namespace Tyra {

class Renderer3D {
 public:
  Renderer3D();
  ~Renderer3D();

  void usePipeline(Renderer3DPipeline* pipeline);

 private:
  Renderer3DPipeline* currentPipeline;
};

}  // namespace Tyra
