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

#include "renderer/3d/pipeline/shared/pipeline_shading_type.hpp"
#include "../shared/pipeline_lighting_options.hpp"
#include "./pipeline_frustum_culling.hpp"

namespace Tyra {

class PipelineOptions {
 public:
  PipelineOptions() {
    lighting = nullptr;
    antiAliasingEnabled = false;
    blendingEnabled = true;
    shadingType = TyraShadingFlat;
  }
  ~PipelineOptions() {}

  PipelineFrustumCulling frustumCulling;
  PipelineShadingType shadingType;
  bool blendingEnabled;
  bool antiAliasingEnabled;

  /** Optional */
  PipelineLightingOptions* lighting;
};

}  // namespace Tyra
