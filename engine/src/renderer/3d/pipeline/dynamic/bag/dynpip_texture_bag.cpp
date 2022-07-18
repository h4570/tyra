/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/dynamic/bag/dynpip_texture_bag.hpp"

namespace Tyra {

DynPipTextureBag::DynPipTextureBag() {
  coordinatesFrom = nullptr;
  coordinatesTo = nullptr;
  texture = nullptr;
}

DynPipTextureBag::~DynPipTextureBag() {}

}  // namespace Tyra
