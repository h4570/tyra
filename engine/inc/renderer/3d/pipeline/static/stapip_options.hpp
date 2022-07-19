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

namespace Tyra {

class StaPipOptions {
 public:
  StaPipOptions() {}
  ~StaPipOptions() {}

  PipelineShadingType shadingType;
  bool blendingEnabled;
  bool antiAliasingEnabled;

  /**
   * @brief True -> disables "clip against each plane" algorithm.
   * Mandatory, default: false.
   *
   * Full clip checks are slow, but they are
   * preventing visual artifacts, which can happen
   * for big 3D objects (or objects near camera eyes)
   */
  bool noFullClipChecks;

  /** Optional */
  PipelineLightingOptions* lighting;
};

}  // namespace Tyra
