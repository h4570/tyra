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

const u32 DynamicPipeline::buffersCount = 64;
const u32 DynamicPipeline::halfBuffersCount = buffersCount / 2;

DynamicPipeline::DynamicPipeline() {
  colorsCache = new Vec4[4];
  buffers = new DynPipBag[buffersCount];
}

DynamicPipeline::~DynamicPipeline() {
  delete[] colorsCache;
  for (u32 i = 0; i < buffersCount; i++) freeBuffer(&buffers[i]);
  delete[] buffers;
}

void DynamicPipeline::init(RendererCore* t_core) {
  rendererCore = t_core;
  auto packetSize = buffersCount * 3.1F;
  core.init(t_core, static_cast<u32>(packetSize));
}

void DynamicPipeline::onUse() { core.reinitVU1Programs(); }

void DynamicPipeline::render(DynamicMesh* mesh, const DynPipOptions* options) {
  auto model = mesh->getModelMatrix();
  auto* infoBag = getInfoBag(mesh, options, &model);
  PipelineDirLightsBag* dirLights = nullptr;
  auto frustumCulling =
      options ? options->frustumCulling : DynPipFrustumCulling_Simple;

  if (frustumCulling == DynPipFrustumCulling_Simple) {
    auto* frameTo = mesh->getFrame(mesh->getNextAnimationFrame());
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

  u16 bufferIndex = 0;

  setBuffersDefaultVars(buffers, mesh, infoBag);
  core.clear();

  for (u32 i = 0; i < mesh->getMaterialsCount(); i++) {
    auto* material = mesh->getMaterial(i);
    auto* frameFrom = material->getFrame(mesh->getCurrentAnimationFrame());
    auto* frameTo = material->getFrame(mesh->getNextAnimationFrame());

    auto partSize =
        core.getMaxVertCountByParams(options && options->lighting,
                                     material->getFrame(0)->getTextureCoords());

    u32 partsCount =
        ceil(frameFrom->getVertexCount() / static_cast<float>(partSize));

    auto* colorBag = getColorBag(material);

    setBuffersColorBag(buffers, colorBag);
    u8 isPartInitialized = false;

    for (u32 k = 0; k < partsCount; k++) {
      auto& buffer = buffers[bufferIndex];

      freeBuffer(&buffer);
      buffer.count = k == partsCount - 1
                         ? frameFrom->getVertexCount() - k * partSize
                         : partSize;

      u32 startIndex = k * partSize;

      addVertices(frameFrom, frameTo, &buffer, startIndex);
      buffer.texture = getTextureBag(material, frameFrom, frameTo, startIndex);
      buffer.lighting = getLightingBag(frameFrom, frameTo, &model, options,
                                       dirLights, startIndex);

      if (!isPartInitialized) {
        core.initParts(&buffer);
        isPartInitialized = true;
      }

      setBuffer(buffers, &buffer, &bufferIndex, frustumCulling);
    }

    delete colorBag;
  }

  sendRestOfBuffers(buffers, &bufferIndex, frustumCulling);

  // TODO: Program renderujacy w VU1
  // TODO: Github: Ten sam myk zrobic w static pipeline
  // TODO: Github: Ten sam myk zrobic w mc pipeline
  // TODO: Github: Allocate dynamic pipeline memory only in "onUse"
  // (core,qbuffrenderer,pipeline..)

  if (dirLights) {
    delete dirLights;
  }

  delete infoBag;
}

void DynamicPipeline::setBuffer(DynPipBag* buffers, DynPipBag* buffer,
                                u16* bufferIndex,
                                const DynPipFrustumCulling& frustumCulling) {
  auto isHalf = *bufferIndex == halfBuffersCount - 1;
  auto isFull = *bufferIndex == buffersCount - 1;

  if (isHalf || isFull) {
    u32 offset = isHalf ? 0 : halfBuffersCount;

    DynPipBag** sendBuffers = new DynPipBag*[halfBuffersCount];

    for (u32 i = 0; i < halfBuffersCount; i++)
      sendBuffers[i] = &buffers[offset + i];

    core.renderPart(sendBuffers, halfBuffersCount,
                    frustumCulling == DynPipFrustumCulling_Precise);

    delete[] sendBuffers;
  }

  if (isFull)
    *bufferIndex = 0;
  else
    *bufferIndex += 1;
}

void DynamicPipeline::sendRestOfBuffers(
    DynPipBag* buffers, u16* bufferIndex,
    const DynPipFrustumCulling& frustumCulling) {
  auto isHalf = *bufferIndex == halfBuffersCount - 1;

  u32 offset = isHalf ? 0 : halfBuffersCount;
  u32 size = *bufferIndex - offset;

  if (size <= 0) return;

  DynPipBag** sendBuffers = new DynPipBag*[size];
  for (u32 i = 0; i < size; i++) {
    sendBuffers[i] = &buffers[offset + i];
  }

  core.renderPart(sendBuffers, size,
                  frustumCulling == DynPipFrustumCulling_Precise);

  delete[] sendBuffers;
}

void DynamicPipeline::setBuffersDefaultVars(DynPipBag* buffers,
                                            DynamicMesh* mesh,
                                            PipelineInfoBag* infoBag) {
  for (u32 i = 0; i < buffersCount; i++) {
    buffers[i].info = infoBag;
    buffers[i].interpolation = mesh->getAnimState().interpolation;
  }
}

void DynamicPipeline::setBuffersColorBag(DynPipBag* buffers,
                                         DynPipColorBag* colorBag) {
  for (u32 i = 0; i < buffersCount; i++) {
    buffers[i].color = colorBag;
  }
}

void DynamicPipeline::freeBuffer(DynPipBag* bag) {
  if (bag->texture) {
    delete bag->texture;
  }

  if (bag->lighting) {
    delete bag->lighting;
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

void DynamicPipeline::addVertices(MeshMaterialFrame* materialFrameFrom,
                                  MeshMaterialFrame* materialFrameTo,
                                  DynPipBag* bag, const u32& startIndex) const {
  bag->verticesFrom = &materialFrameFrom->getVertices()[startIndex];
  bag->verticesTo = &materialFrameTo->getVertices()[startIndex];
}

DynPipColorBag* DynamicPipeline::getColorBag(MeshMaterial* material) const {
  auto* result = new DynPipColorBag();
  result->single = &material->color;
  return result;
}

DynPipTextureBag* DynamicPipeline::getTextureBag(
    MeshMaterial* material, MeshMaterialFrame* materialFrameFrom,
    MeshMaterialFrame* materialFrameTo, const u32& startIndex) {
  if (!materialFrameFrom->getTextureCoords()) return nullptr;

  auto* result = new DynPipTextureBag();

  result->texture =
      rendererCore->texture.repository.getBySpriteOrMesh(material->getId());
  TYRA_ASSERT(result->texture, "Texture for material id: ", material->getId(),
              "was not found in texture repository!");

  result->coordinatesFrom = &materialFrameFrom->getTextureCoords()[startIndex];
  result->coordinatesTo = &materialFrameTo->getTextureCoords()[startIndex];

  return result;
}

DynPipLightingBag* DynamicPipeline::getLightingBag(
    MeshMaterialFrame* materialFrameFrom, MeshMaterialFrame* materialFrameTo,
    M4x4* model, const DynPipOptions* options,
    PipelineDirLightsBag* dirLightsBag, const u32& startIndex) const {
  if (!materialFrameFrom->getNormals() || options == nullptr ||
      options->lighting == nullptr)
    return nullptr;

  auto* result = new DynPipLightingBag();

  result->lightMatrix = model;
  result->dirLights = dirLightsBag;

  result->normalsFrom = &materialFrameFrom->getNormals()[startIndex];
  result->normalsTo = &materialFrameTo->getNormals()[startIndex];

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
