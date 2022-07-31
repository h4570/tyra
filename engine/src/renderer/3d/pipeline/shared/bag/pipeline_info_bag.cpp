/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/shared/bag/pipeline_info_bag.hpp"

namespace Tyra {

PipelineInfoBag::PipelineInfoBag() {
  shadingType = TyraShadingFlat;
  transformationType = TyraMVP;
  textureMappingType = TyraLinear;
  blendingEnabled = true;
  antiAliasingEnabled = false;
  model = nullptr;
}

PipelineInfoBag::~PipelineInfoBag() {}

}  // namespace Tyra
