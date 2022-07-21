/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/dynamic/dynamic_pipeline.hpp"

namespace Tyra {

DynamicPipeline::DynamicPipeline() { colorsCache = new Vec4[4]; }

DynamicPipeline::~DynamicPipeline() { delete[] colorsCache; }

void DynamicPipeline::init(RendererCore* t_core) {
  rendererCore = t_core;
  core.init(t_core);
}

void DynamicPipeline::onUse() { core.reinitVU1Programs(); }

void DynamicPipeline::render(DynamicMesh* mesh, const DynPipOptions* options) {
  auto model = mesh->getModelMatrix();
  auto* frameFrom = mesh->getFrame(mesh->getCurrentAnimationFrame());
  auto* frameTo = mesh->getFrame(mesh->getNextAnimationFrame());
  auto* infoBag = getInfoBag(mesh, options, &model);
  PipelineDirLightsBag* dirLights = nullptr;
  auto frustumCulling =
      options ? options->frustumCulling : DynPipFrustumCulling_Simple;

  if (frustumCulling == DynPipFrustumCulling_Simple) {
    if (frameTo->getBBox().isInFrustum(
            rendererCore->renderer3D.frustumPlanes.getAll(), model) ==
        CoreBBoxFrustum::OUTSIDE_FRUSTUM) {
      return;
    }
  }

  if (options && options->lighting) {
    setLightingColorsCache(options->lighting);
    dirLights = new PipelineDirLightsBag(true);
    dirLights->setLightsManually(colorsCache,
                                 options->lighting->directionalDirections);
  }

  DynPipBag buffers[2];
  u8 context = 0;

  setBuffersDefaultVars(buffers, mesh, infoBag);
  core.clear();

  for (u32 i = 0; i < mesh->getMaterialsCount(); i++) {
    auto* material = mesh->getMaterial(i);
    auto partSize = core.getMaxVertCountByParams(
        options && options->lighting, material->getTextureCoordFaces());
    u32 partsCount =
        ceil(material->getFacesCount() / static_cast<float>(partSize));
    auto* colorBag = getColorBag(material);
    setBuffersColorBag(buffers, colorBag);
    u8 isPartInitialized = false;

    for (u32 j = 0; j < partsCount; j++) {
      auto& buffer = buffers[context];

      freeBuffer(&buffer);
      buffer.count = j == partsCount - 1
                         ? material->getFacesCount() - j * partSize
                         : partSize;

      u32 startIndex = j * partSize;

      addVertices(mesh, material, &buffer, frameFrom, frameTo, startIndex);
      buffer.texture = getTextureBag(mesh, material, frameFrom, frameTo,
                                     buffer.count, startIndex);
      buffer.lighting =
          getLightingBag(mesh, material, &model, frameFrom, frameTo, options,
                         dirLights, buffer.count, startIndex);

      if (!isPartInitialized) {
        core.initParts(&buffer);
        isPartInitialized = true;
      }

      core.renderPart(&buffer, frustumCulling == DynPipFrustumCulling_Precise);

      context = !context;
    }

    freeBuffer(&buffers[context]);
    freeBuffer(&buffers[!context]);

    delete colorBag;
  }

  delete infoBag;
}

void DynamicPipeline::setBuffersDefaultVars(DynPipBag* buffers,
                                            DynamicMesh* mesh,
                                            PipelineInfoBag* infoBag) {
  for (int i = 0; i < 2; i++) {
    buffers[i].info = infoBag;
    buffers[i].interpolation = mesh->getAnimState().interpolation;
  }
}

void DynamicPipeline::setBuffersColorBag(DynPipBag* buffers,
                                         DynPipColorBag* colorBag) {
  for (int i = 0; i < 2; i++) {
    buffers[i].color = colorBag;
  }
}

void DynamicPipeline::freeBuffer(DynPipBag* bag) {
  if (bag->texture) {
    bag->texture->freeCoords();
    delete bag->texture;
  }

  if (bag->lighting) {
    bag->lighting->freeNormals();
    delete bag->lighting;
  }

  if (bag->verticesFrom) {
    delete[] bag->verticesFrom;
  }

  if (bag->verticesTo) {
    delete[] bag->verticesTo;
  }
}

PipelineInfoBag* DynamicPipeline::getInfoBag(DynamicMesh* mesh,
                                             const DynPipOptions* options,
                                             M4x4* model) const {
  auto* result = new PipelineInfoBag();

  if (options) {
    result->antiAliasingEnabled = options->antiAliasingEnabled;
    result->blendingEnabled = options->blendingEnabled;
    result->shadingType = options->shadingType;
  } else {
    result->antiAliasingEnabled = false;
    result->blendingEnabled = true;
    result->shadingType = TyraShadingFlat;
  }

  result->model = model;

  return result;
}

void DynamicPipeline::addVertices(DynamicMesh* mesh, MeshMaterial* material,
                                  DynPipBag* bag, MeshFrame* frameFrom,
                                  MeshFrame* frameTo,
                                  const u32& startIndex) const {
  bag->verticesFrom = new Vec4[bag->count];
  bag->verticesTo = new Vec4[bag->count];

  for (u32 i = startIndex; i < startIndex + bag->count; i++) {
    auto& face = material->getVertexFaces()[i];
    auto offset = i - startIndex;
    bag->verticesFrom[offset] = frameFrom->getVertices()[face];
    bag->verticesTo[offset] = frameTo->getVertices()[face];
  }
}

DynPipColorBag* DynamicPipeline::getColorBag(MeshMaterial* material) const {
  auto* result = new DynPipColorBag();
  result->single = &material->singleColor;
  return result;
}

DynPipTextureBag* DynamicPipeline::getTextureBag(
    DynamicMesh* mesh, MeshMaterial* material, MeshFrame* frameFrom,
    MeshFrame* frameTo, const u32& count, const u32& startIndex) {
  if (!material->getTextureCoordFaces()) return nullptr;

  auto* result = new DynPipTextureBag();
  result->texture =
      rendererCore->texture.repository.getBySpriteOrMesh(material->getId());
  TYRA_ASSERT(result->texture, "Texture for material id: ", material->getId(),
              "was not found in texture repository!");

  result->coordinatesFrom = new Vec4[count];
  result->coordinatesTo = new Vec4[count];

  for (u32 i = startIndex; i < startIndex + count; i++) {
    auto& face = material->getTextureCoordFaces()[i];
    auto offset = i - startIndex;
    result->coordinatesFrom[offset] = frameFrom->getTextureCoords()[face];
    result->coordinatesTo[offset] = frameTo->getTextureCoords()[face];
  }

  return result;
}

DynPipLightingBag* DynamicPipeline::getLightingBag(
    DynamicMesh* mesh, MeshMaterial* material, M4x4* model,
    MeshFrame* frameFrom, MeshFrame* frameTo, const DynPipOptions* options,
    PipelineDirLightsBag* dirLightsBag, const u32& count,
    const u32& startIndex) const {
  if (!material->getNormalFaces() || options == nullptr ||
      options->lighting == nullptr)
    return nullptr;

  auto* result = new DynPipLightingBag();

  result->lightMatrix = model;
  result->dirLights = dirLightsBag;
  result->normalsFrom = new Vec4[count];
  result->normalsTo = new Vec4[count];

  for (u32 i = startIndex; i < startIndex + count; i++) {
    auto& face = material->getNormalFaces()[i];
    auto offset = i - startIndex;
    result->normalsFrom[offset] = frameFrom->getNormals()[face];
    result->normalsTo[offset] = frameTo->getNormals()[face];
  }

  return result;
}

void DynamicPipeline::setLightingColorsCache(
    PipelineLightingOptions* lightingOptions) {
  for (int i = 0; i < 3; i++) {
    colorsCache[i] =
        reinterpret_cast<Vec4&>(lightingOptions->directionalColors[i]);
  }
  colorsCache[3] = reinterpret_cast<Vec4&>(*lightingOptions->ambientColor);
}

}  // namespace Tyra
