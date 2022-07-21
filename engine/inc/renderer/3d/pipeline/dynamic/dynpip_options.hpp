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

enum DynPipFrustumCulling {
  DynPipFrustumCulling_None = 0,
  DynPipFrustumCulling_Simple = 1,
  DynPipFrustumCulling_Precise = 2,
};

class DynPipOptions : public PipelineOptions {
 public:
  DynPipOptions() {}
  ~DynPipOptions() {}

  /** Default: Simple */
  DynPipFrustumCulling frustumCulling;
};

}  // namespace Tyra
