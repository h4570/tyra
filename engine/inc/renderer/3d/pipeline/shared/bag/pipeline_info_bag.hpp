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

#include <draw.h>
#include "math/m4x4.hpp"
#include "math/vec4.hpp"
#include "../pipeline_shading_type.hpp"
#include "../pipeline_texture_mapping_type.hpp"
#include "../pipeline_transformation_type.hpp"

namespace Tyra {

class PipelineInfoBag {
 public:
  PipelineInfoBag();
  ~PipelineInfoBag();

  /** Mandatory. Model matrix */
  M4x4* model;

  PipelineShadingType shadingType;
  PipelineTextureMappingType textureMappingType;
  PipelineTransformationType transformationType;
  bool blendingEnabled;
  bool antiAliasingEnabled;

  /**
   * @brief False -> disables "clip against each plane" algorithm.
   * Default: True.
   *
   * Full clip checks are slow, but they are
   * preventing visual artifacts, which can happen
   * for big 3D objects (or objects near camera eyes)
   * Force enabled in dynamic pipe, because of efficiency.
   */
  bool fullClipChecks;
};

}  // namespace Tyra
