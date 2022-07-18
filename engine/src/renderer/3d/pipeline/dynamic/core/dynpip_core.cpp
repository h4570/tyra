/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/dynamic/core/dynpip_core.hpp"
#include <sstream>
#include <iomanip>

namespace Tyra {

DynPipCore::DynPipCore() {}

DynPipCore::~DynPipCore() {}

void DynPipCore::init(RendererCore* t_core) { rendererCore = t_core; }

void DynPipCore::reinitStandardVU1Programs() {}

void DynPipCore::render(DynPipBag* bag) {}

}  // namespace Tyra
