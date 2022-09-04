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

#include "./pipeline_shading_type.hpp"
#include "./pipeline_lighting_options.hpp"
#include "./pipeline_texture_mapping_type.hpp"
#include "./pipeline_transformation_type.hpp"
#include "./pipeline_z_test.hpp"
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
    zTestType = PipelineZTest_Standard;
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

  /** Type of z-buffer testing. */
  PipelineZTest zTestType;

  /** Multiply by model matrix by view-projection or projection matrix */
  PipelineTransformationType transformationType;

  /** Optional */
  PipelineLightingOptions* lighting;
};

}  // namespace Tyra
