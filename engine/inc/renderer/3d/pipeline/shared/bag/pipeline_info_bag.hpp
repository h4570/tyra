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

#include <draw.h>
#include "math/m4x4.hpp"
#include "math/vec4.hpp"
#include "../pipeline_shading_type.hpp"
#include "../pipeline_texture_mapping_type.hpp"
#include "../pipeline_transformation_type.hpp"
#include "./pipeline_info_bag_frustum_culling.hpp"

namespace Tyra {

class PipelineInfoBag {
 public:
  PipelineInfoBag() {
    shadingType = TyraShadingFlat;
    transformationType = TyraMVP;
    textureMappingType = TyraLinear;
    blendingEnabled = true;
    antiAliasingEnabled = false;
    model = nullptr;
    frustumCulling = PipelineInfoBagFrustumCulling_None;
  }
  ~PipelineInfoBag() {}

  /** Mandatory. Model matrix */
  M4x4* model;

  /** Flat or gouraud */
  PipelineShadingType shadingType;

  /** Linear or nearest */
  PipelineTextureMappingType textureMappingType;

  /** Multiply by model matrix by view-projection or projection matrix */
  PipelineTransformationType transformationType;

  /** Blending texture with color */
  bool blendingEnabled;

  /** Anti-aliasing */
  bool antiAliasingEnabled;

  /** Type of frustum culling */
  PipelineInfoBagFrustumCulling frustumCulling;
};

}  // namespace Tyra
