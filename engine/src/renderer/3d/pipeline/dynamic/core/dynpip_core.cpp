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

void DynPipCore::init(RendererCore* t_core) {
  rendererCore = t_core;
  qbufferRenderer.init(t_core, &repository);
}

void DynPipCore::reinitVU1Programs() { qbufferRenderer.reinitVU1(); }

u32 DynPipCore::getMaxVertCountByParams(const bool& isSingleColor,
                                        const bool& isLightingEnabled,
                                        const bool& isTextureEnabled) {
  return repository.getProgramByParams(isLightingEnabled, isTextureEnabled)
      ->getMaxVertCount(isSingleColor, qbufferRenderer.getBufferSize());
}

void DynPipCore::initParts(DynPipBag* bag) {
  RendererCoreTextureBuffers* texBuffers = nullptr;
  if (bag->texture) {
    auto temp = rendererCore->texture.useTexture(bag->texture->texture);
    texBuffers = new RendererCoreTextureBuffers{temp.id, temp.core, temp.clut};
  }

  mvp = rendererCore->renderer3D.getViewProj() * *bag->info->model;

  qbufferRenderer.sendObjectData(bag, &mvp, texBuffers);

  delete texBuffers;
}

void DynPipCore::renderPart(DynPipBag* bag, const bool& frustumCull) {
  if (bag->count <= 0) return;

  if (frustumCull) {
    CoreBBox bbox(bag->verticesTo, bag->count);
    if (bbox.isInFrustum(rendererCore->renderer3D.frustumPlanes.getAll(),
                         *bag->info->model) ==
        CoreBBoxFrustum::OUTSIDE_FRUSTUM) {
      return;
    }
  }

  // TODO: Uruchomic odpowiedni program
  // TODO: Wyslac wierzcholki
  // TODO: Program VU1 TD
  // TODO: Reszta programow VU1
}

}  // namespace Tyra
