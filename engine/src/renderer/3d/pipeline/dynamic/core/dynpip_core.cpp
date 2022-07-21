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

void DynPipCore::reinitVU1Programs() {}

u32 DynPipCore::getMaxVertCountByParams(const bool& isSingleColor,
                                        const bool& isLightingEnabled,
                                        const bool& isTextureEnabled) {
  return repository.getProgramByParams(isLightingEnabled, isTextureEnabled)
      ->getMaxVertCount(isSingleColor, qbufferRenderer.getBufferSize());
}

void DynPipCore::render(DynPipBag* bag) {
  // Zrobic bbox
  // TODO: Potem wrzucic programy i obliczyc bufferSize w qbufferrenderer
}

}  // namespace Tyra
