/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/std/core/bag/stdpip_info_bag.hpp"

namespace Tyra {

StdpipInfoBag::StdpipInfoBag() {
  shadingType = StdpipShadingFlat;
  blendingEnabled = true;
  antiAliasingEnabled = false;
  model = nullptr;
}

StdpipInfoBag::~StdpipInfoBag() {}

}  // namespace Tyra
