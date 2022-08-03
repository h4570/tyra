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

#include "../shared/pipeline_shading_type.hpp"
#include "../shared/pipeline_lighting_options.hpp"
#include "../shared/pipeline_texture_mapping_type.hpp"
#include "../shared/pipeline_transformation_type.hpp"
#include "./pipeline_frustum_culling.hpp"

namespace Tyra {

class PipelineOptions {
 public:
  PipelineOptions() {
    lighting = nullptr;
    antiAliasingEnabled = false;
    blendingEnabled = true;
    shadingType = TyraShadingFlat;
    textureMappingType = TyraLinear;
    transformationType = TyraMVP;
  }
  ~PipelineOptions() {}

  /** Type of frustum culling */
  PipelineFrustumCulling frustumCulling;

  /** Flat or gouraud */
  PipelineShadingType shadingType;

  /** Linear or nearest */
  PipelineTextureMappingType textureMappingType;

  /** Blending texture with color */
  bool blendingEnabled;

  /** Anti-aliasing */
  bool antiAliasingEnabled;

  /** Multiply by model matrix by view-projection or projection matrix */
  PipelineTransformationType transformationType;

  /** Optional */
  PipelineLightingOptions* lighting;
};

}  // namespace Tyra
