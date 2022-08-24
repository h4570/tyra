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

#include "./pipeline/renderer_3d_pipeline.hpp"
#include "./renderer_3d_utility.hpp"
#include "../core/renderer_core.hpp"

namespace Tyra {

class Renderer3D {
 public:
  Renderer3D();
  ~Renderer3D();

  Renderer3DUtility utility;

  /** Deinitialize previous pipeline and initialize new pipeline */
  void usePipeline(Renderer3DPipeline* pipeline);
  void usePipeline(Renderer3DPipeline& pipeline);

  /** Called by engine */
  void init(RendererCore* core);

  /** Called by engine */
  void onFrameEnd();

 private:
  Renderer3DPipeline* currentPipeline;
  void onDestroy(Renderer3DPipeline* pipeline);
};

}  // namespace Tyra
