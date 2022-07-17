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
#include "../../stdpip_shading_type.hpp"

namespace Tyra {

class StdpipInfoBag {
 public:
  StdpipInfoBag();
  ~StdpipInfoBag();

  /** Mandatory. Model matrix */
  M4x4* model;

  StdpipShadingType shadingType;
  bool blendingEnabled;
  bool antiAliasingEnabled;
  bool noClipChecks;
};

}  // namespace Tyra
