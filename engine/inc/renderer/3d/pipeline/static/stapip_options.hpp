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

#include "renderer/3d/pipeline/shared/pipeline_options.hpp"

namespace Tyra {

class StaPipOptions : public PipelineOptions {
 public:
  StaPipOptions() { fullClipChecks = false; }
  ~StaPipOptions() {}

  /**
   * @brief Experimental! True -> enables "clip against each plane" algorithm.
   * Mandatory, default: False.
   *
   * Full clip checks are slow, but they are
   * preventing visual artifacts, which can happen
   * for big 3D objects (or objects near camera eyes).
   *
   * There is other way to prevent these glitches. Just convert your triangles
   * to smaller triangles (for example 1 big triangle to 10 smaller). It will
   * be way faster than enabling this option...
   */
  bool fullClipChecks;
};

}  // namespace Tyra
