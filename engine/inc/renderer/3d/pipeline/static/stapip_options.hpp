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

#include "./stapip_shading_type.hpp"
#include "./stapip_lighting_options.hpp"

namespace Tyra {

class StapipOptions {
 public:
  StapipOptions() {}
  ~StapipOptions() {}

  StapipShadingType shadingType;
  bool blendingEnabled;
  bool antiAliasingEnabled;
  bool noClipChecks;

  /** Optional */
  StapipLightingOptions* lighting;
};

}  // namespace Tyra
