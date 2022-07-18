/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/static/core/bag/stapip_info_bag.hpp"

namespace Tyra {

StapipInfoBag::StapipInfoBag() {
  shadingType = StapipShadingFlat;
  blendingEnabled = true;
  antiAliasingEnabled = false;
  model = nullptr;
}

StapipInfoBag::~StapipInfoBag() {}

}  // namespace Tyra
