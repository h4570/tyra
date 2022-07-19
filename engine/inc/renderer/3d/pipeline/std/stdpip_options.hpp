/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "./stdpip_shading_type.hpp"
#include "./stdpip_lighting_options.hpp"

namespace Tyra {

class StdpipOptions {
 public:
  StdpipOptions() {}
  ~StdpipOptions() {}

  StdpipShadingType shadingType;
  bool blendingEnabled;
  bool antiAliasingEnabled;
  bool noClipChecks;

  /** Optional */
  StdpipLightingOptions* lighting;
};

}  // namespace Tyra
