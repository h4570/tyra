/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/renderer/3d/pipeline/static/core/bag/stapip_lighting_bag.hpp"

namespace Tyra {

StaPipLightingBag::StaPipLightingBag() {
  lightMatrix = nullptr;
  normals = nullptr;
  dirLights = nullptr;
}

StaPipLightingBag::~StaPipLightingBag() {}

}  // namespace Tyra
