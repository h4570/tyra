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

DynPipCore::DynPipCore() {
  setPrim();
  setLod();
}

DynPipCore::~DynPipCore() {}

void DynPipCore::init(RendererCore* t_core) {
  rendererCore = t_core;
  qbufferRenderer.init(t_core, &repository, &prim, &lod);
}

void DynPipCore::setPrim() {
  prim.type = PRIM_TRIANGLE;
  prim.shading = PRIM_SHADE_GOURAUD;
  prim.mapping = DRAW_ENABLE;
  prim.fogging = DRAW_DISABLE;
  prim.blending = DRAW_ENABLE;
  prim.antialiasing = DRAW_DISABLE;
  prim.mapping_type = PRIM_MAP_ST;
  prim.colorfix = PRIM_UNFIXED;
}

void DynPipCore::setLod() {
  lod.calculation = LOD_USE_K;
  lod.max_level = 0;
  lod.mag_filter = LOD_MAG_LINEAR;
  lod.min_filter = LOD_MIN_LINEAR;
  lod.mipmap_select = LOD_MIPMAP_REGISTER;
  lod.l = 0;
  lod.k = 0.0F;
}

void DynPipCore::reinitVU1Programs() { qbufferRenderer.reinitVU1(); }

u32 DynPipCore::getMaxVertCountByParams(const bool& isLightingEnabled,
                                        const bool& isTextureEnabled) {
  return repository.getProgramByParams(isLightingEnabled, isTextureEnabled)
      ->getMaxVertCount(qbufferRenderer.getBufferSize());
}

void DynPipCore::sendObjectDataToVU1(DynPipBag* bag) {
  RendererCoreTextureBuffers* texBuffers = nullptr;
  if (bag->texture) {
    auto temp = rendererCore->texture.useTexture(bag->texture->texture);
    texBuffers = new RendererCoreTextureBuffers{temp.id, temp.core, temp.clut};
  }

  if (bag->info->transformationType == TyraMP) {
    mvp = rendererCore->renderer3D.getProjection() * *bag->info->model;
  } else {
    mvp = rendererCore->renderer3D.getViewProj() * *bag->info->model;
  }

  qbufferRenderer.sendObjectData(bag, &mvp, texBuffers);

  delete texBuffers;
}

void DynPipCore::begin(PipelineInfoBag* bag) {
  qbufferRenderer.clearLastProgramName();

  prim.antialiasing = bag->antiAliasingEnabled;
  prim.blending = bag->blendingEnabled;
  prim.shading = bag->shadingType;

  if (bag->textureMappingType == TyraLinear) {
    lod.mag_filter = LOD_MAG_LINEAR;
    lod.min_filter = LOD_MIN_LINEAR;
  } else {
    lod.mag_filter = LOD_MAG_NEAREST;
    lod.min_filter = LOD_MIN_NEAREST;
  }
}

void DynPipCore::render(DynPipBag** bags, const u32& count) {
  if (count <= 0) return;

  TYRA_ASSERT(
      bags[0]->verticesFrom != nullptr && bags[0]->verticesTo != nullptr,
      "Vertices are required in 3D render bag!");
  TYRA_ASSERT(bags[0]->info != nullptr,
              "Info bag is required in 3D render bag!");
  TYRA_ASSERT(bags[0]->info->model != nullptr,
              "Info bag's model pointer is empty!");
  TYRA_ASSERT(bags[0]->color != nullptr,
              "Color bag is required in 3D render bag!");
  TYRA_ASSERT(bags[0]->color->single, "Color is required in 3D render bag!");
  TYRA_ASSERT(
      !bags[0]->lighting ||
          (bags[0]->lighting->lightMatrix && bags[0]->lighting->normalsFrom &&
           bags[0]->lighting->normalsTo && bags[0]->lighting->dirLights),
      "If you want lighting, please provide light matrix normals and dir "
      "lights!");
  TYRA_ASSERT(!bags[0]->texture || (bags[0]->texture->texture &&
                                    bags[0]->texture->coordinatesFrom &&
                                    bags[0]->texture->coordinatesTo),
              "If you want texture, please provide texture and coordinates!");

  if (bags[0]->info->frustumCulling == PipelineInfoBagFrustumCulling_None) {
    qbufferRenderer.render(bags, count);
    return;
  }

  DynPipBag** bagsToRender = new DynPipBag*[count];
  u32 inserted = 0;

  for (u32 i = 0; i < count; i++) {
    auto* bag = bags[i];

    CoreBBox bbox(bag->verticesTo, bag->count);
    if (bbox.isInFrustum(rendererCore->renderer3D.frustumPlanes.getAll(),
                         *bag->info->model) ==
        CoreBBoxFrustum::OUTSIDE_FRUSTUM) {
      continue;
    }

    bagsToRender[inserted++] = bag;
  }

  qbufferRenderer.render(bagsToRender, inserted);
  delete[] bagsToRender;
}

}  // namespace Tyra
