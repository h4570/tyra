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
#include "thread/threading.hpp"

namespace Tyra {

DynPipCore::DynPipCore() {}

DynPipCore::~DynPipCore() {}

void DynPipCore::init(RendererCore* t_core) {
  rendererCore = t_core;
  qbufferRenderer.init(t_core, &repository);
}

void DynPipCore::reinitVU1Programs() { qbufferRenderer.reinitVU1(); }

u32 DynPipCore::getMaxVertCountByParams(const bool& isLightingEnabled,
                                        const bool& isTextureEnabled) {
  return repository.getProgramByParams(isLightingEnabled, isTextureEnabled)
      ->getMaxVertCount(qbufferRenderer.getBufferSize());
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

  TYRA_ASSERT(bag->verticesFrom != nullptr && bag->verticesTo != nullptr,
              "Vertices are required in 3D render bag!");
  TYRA_ASSERT(bag->info != nullptr, "Info bag is required in 3D render bag!");
  TYRA_ASSERT(bag->info->model != nullptr,
              "Info bag's model pointer is empty!");
  TYRA_ASSERT(bag->color != nullptr, "Color bag is required in 3D render bag!");
  TYRA_ASSERT(bag->color->single, "Color is required in 3D render bag!");
  TYRA_ASSERT(
      !bag->lighting ||
          (bag->lighting->lightMatrix && bag->lighting->normalsFrom &&
           bag->lighting->normalsTo && bag->lighting->dirLights),
      "If you want lighting, please provide light matrix normals and dir "
      "lights!");
  TYRA_ASSERT(!bag->texture ||
                  (bag->texture->texture && bag->texture->coordinatesFrom &&
                   bag->texture->coordinatesTo),
              "If you want texture, please provide texture and coordinates!");

  if (frustumCull) {
    CoreBBox bbox(bag->verticesTo, bag->count);
    if (bbox.isInFrustum(rendererCore->renderer3D.frustumPlanes.getAll(),
                         *bag->info->model) ==
        CoreBBoxFrustum::OUTSIDE_FRUSTUM) {
      return;
    }
  }

  qbufferRenderer.render(bag);
}

}  // namespace Tyra
