/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/static/core/static_pipeline_core.hpp"
#include "renderer/core/renderer_core.hpp"
#include "thread/threading.hpp"

// #define TYRA_RENDERER_VERBOSE_LOG 1

#ifdef TYRA_RENDERER_VERBOSE_LOG
#define Verbose(...) Debug::writeLines("VRB: ", ##__VA_ARGS__, "\n")
#else
#define Verbose(...) ((void)0)
#endif

namespace Tyra {

StapipelineCore::StapipelineCore() { maxVertCount = 0; }

StapipelineCore::~StapipelineCore() {}

void StapipelineCore::init(RendererCore* t_core) {
  rendererCore = t_core;
  qbufferRenderer.init(t_core);
  packager.init(&rendererCore->renderer3D.frustumPlanes);
}

void StapipelineCore::reinitStandardVU1Programs() {
  qbufferRenderer.reinitVU1();
}

u32 StapipelineCore::getMaxVertCountByBag(const StapipBag* bag) {
  return qbufferRenderer.getCullProgramByBag(bag)->getMaxVertCount(
      bag->color->many == nullptr, qbufferRenderer.getBufferSize());
}

u32 StapipelineCore::getMaxVertCountByParams(const bool& isSingleColor,
                                             const bool& isLightingEnabled,
                                             const bool& isTextureEnabled) {
  return qbufferRenderer
      .getCullProgramByParams(isLightingEnabled, isTextureEnabled)
      ->getMaxVertCount(isSingleColor, qbufferRenderer.getBufferSize());
}

void StapipelineCore::render(StapipBag* bag, StapipBagPackagesBBox* bbox) {
  if (bag->count <= 0) return;

  TYRA_ASSERT(bag->vertices != nullptr,
              "Vertices are required in 3D render bag!");
  TYRA_ASSERT(bag->info != nullptr, "Info bag is required in 3D render bag!");
  TYRA_ASSERT(bag->info->model != nullptr,
              "Info bag's model pointer is empty!");
  TYRA_ASSERT(bag->color != nullptr, "Color bag is required in 3D render bag!");
  TYRA_ASSERT(bag->color->single || bag->color->many,
              "At least one color is required in 3D render bag!");
  TYRA_ASSERT((!bag->color->many && !bag->lighting) ||
                  (bag->color->many && !bag->lighting) ||
                  (!bag->color->many && bag->lighting),
              "Multicolor is not supported with lighting, please choose one!");
  TYRA_ASSERT(
      !bag->lighting || (bag->lighting->lightMatrix && bag->lighting->normals),
      "If you want lighting, please provide light matrix and normals!");
  TYRA_ASSERT(
      !bag->texture || (bag->texture->texture && bag->texture->coordinates),
      "If you want texture, please provide texture and coordinates!");

  StapipBagPackagesBBox* renderBbox;

  u32 maxVertCount = getMaxVertCountByBag(bag);

  setMaxVertCount(maxVertCount);

  if (!bbox)
    renderBbox =
        new StapipBagPackagesBBox(bag->vertices, bag->count, maxVertCount);
  else
    renderBbox = bbox;

  auto frustumCheck = renderBbox->getMainBBox()->clipIsInFrustum(
      rendererCore->renderer3D.frustumPlanes.getAll(), *bag->info->model);
  auto mvp = rendererCore->renderer3D.getViewProj() * *bag->info->model;

  if (frustumCheck == OUTSIDE_FRUSTUM) return;

  RendererCoreTextureBuffers* texBuffers = nullptr;
  if (bag->texture) {
    auto temp = rendererCore->texture.useTexture(bag->texture->texture);
    texBuffers = new RendererCoreTextureBuffers{temp.id, temp.core, temp.clut};
  }

  qbufferRenderer.clearLastProgramName();

  qbufferRenderer.sendObjectData(bag, &mvp, texBuffers);

  packager.setRenderBBox(renderBbox);

  qbufferRenderer.setClipperMVP(&mvp);

  qbufferRenderer.setInfo(bag->info);

  if (frustumCheck == IN_FRUSTUM ||
      (frustumCheck == PARTIALLY_IN_FRUSTUM && bag->info->noClipChecks)) {
    u16 packagesCount = 0;
    auto biggerPkgs = packager.create(&packagesCount, bag, maxVertCount);
    Verbose("Material - in frustum. Pkgs: ", packagesCount,
            " size: ", static_cast<int>(biggerPkgs[0].size));
    for (u16 i = 0; i < packagesCount; i++) {
      Verbose(i, " package - cull by data pointer");
      auto buffer = qbufferRenderer.getBuffer();
      buffer->fillByPointer(biggerPkgs[i]);
      qbufferRenderer.cull(buffer);
    }
    delete[] biggerPkgs;
  } else if (frustumCheck == PARTIALLY_IN_FRUSTUM) {
    u16 packagesCount = 0;
    if (bag->count >= maxVertCount * 2) {
      auto packages = packager.create(&packagesCount, bag, maxVertCount);
      Verbose("Material - partial. Packages: ", packagesCount);
      renderPkgs(packages, packagesCount);
      delete[] packages;
    } else {
      auto subpkgs = packager.create(&packagesCount, bag, maxVertCount / 3);
      Verbose("Material - partial. Subpackages: ", packagesCount);
      renderSubpkgs(subpkgs, packagesCount);
      delete[] subpkgs;
    }
  }

  if (!bbox) delete renderBbox;
  if (texBuffers) delete texBuffers;

  Threading::switchThread();

  Verbose("Render finished");
}

void StapipelineCore::renderPkgs(StapipBagPackage* packages, u16 count) {
  for (u16 i = 0; i < count; i++) {
    if (packages[i].isInFrustum == IN_FRUSTUM) {
      Verbose(i, " - package in frustum -> cull");
      auto buffer = qbufferRenderer.getBuffer();
      buffer->fillByPointer(packages[i]);
      qbufferRenderer.cull(buffer);
    } else if (packages[i].isInFrustum == PARTIALLY_IN_FRUSTUM) {
      u16 subpkgsSize = 0;
      auto packages1By3 =
          packager.create(&subpkgsSize, packages[i], maxVertCount / 3);
      Verbose(i, " - partial package. Created subpkgs: ", subpkgsSize);

      renderSubpkgs(packages1By3, subpkgsSize);
      delete[] packages1By3;
    }
    Verbose(i, " - package skipped (outside)");
  }
}

void StapipelineCore::renderSubpkgs(StapipBagPackage* subpkgs, u16 count) {
  std::vector<u16> doneIndexes;
  std::vector<u16> loadedIndexes;

  // Check if some subpkgs are full in frustum
  for (u16 i = 0; i < count; i++) {
    if (subpkgs[i].isInFrustum == IN_FRUSTUM) {
      if (loadedIndexes.size() <= 1) {
        Verbose(i, " - subpackage in frustum -> load");
        loadedIndexes.push_back(i);
      } else {  // Hmm, this will never happen?
        Verbose(i, " - subpackage in frustum, cull all 3 subpkgs");
        auto buffer = qbufferRenderer.getBuffer();
        buffer->fillByCopyMax(subpkgs[loadedIndexes[0]],
                              subpkgs[loadedIndexes[1]], subpkgs[i]);
        qbufferRenderer.cull(buffer);
        doneIndexes.push_back(loadedIndexes[0]);
        doneIndexes.push_back(loadedIndexes[1]);
        doneIndexes.push_back(i);
        loadedIndexes.clear();
      }
    }
  }

  if (loadedIndexes.size() == 2) {
    Verbose("2 in frustum subpkgs left -> cull them");
    auto buffer = qbufferRenderer.getBuffer();
    buffer->fillByCopy1By2(subpkgs[loadedIndexes[0]],
                           subpkgs[loadedIndexes[1]]);
    qbufferRenderer.cull(buffer);
    doneIndexes.push_back(loadedIndexes[0]);
    doneIndexes.push_back(loadedIndexes[1]);
  } else if (loadedIndexes.size() == 1) {
    Verbose("1 in frustum subpkg left -> cull it");
    auto buffer = qbufferRenderer.getBuffer();
    buffer->fillByPointer(subpkgs[loadedIndexes[0]]);
    qbufferRenderer.cull(buffer);
    doneIndexes.push_back(loadedIndexes[0]);
  }

  for (u16 i = 0; i < count; i++) {
    bool isSkip = subpkgs[i].isInFrustum == OUTSIDE_FRUSTUM ||
                  std::find(doneIndexes.begin(), doneIndexes.end(), i) !=
                      doneIndexes.end();

    if (isSkip) {
      Verbose(i, " - subpkg skipped, already rendered/outside");
      continue;
    }

    auto buffer = qbufferRenderer.getBuffer();
    buffer->fillByCopy1By3(subpkgs[i]);
    Verbose(i, " - subpkg out/partial -> send to clipper");
    qbufferRenderer.clip(buffer);
  }
}

void StapipelineCore::setMaxVertCount(const u32& count) {
  maxVertCount = count;
  packager.setMaxVertCount(count);
  qbufferRenderer.setMaxVertCount(count);
}

}  // namespace Tyra
