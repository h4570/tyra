/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/renderer/3d/pipeline/dynamic/dynamic_pipeline.hpp"
#include "shared/renderer/3d/pipeline/shared/bag/pipeline_info_bag.hpp"

namespace Tyra {

const unsigned int DynamicPipeline::buffersCount = 64;
const unsigned int DynamicPipeline::halfBuffersCount = buffersCount / 2;

DynamicPipeline::DynamicPipeline() {}

DynamicPipeline::~DynamicPipeline() {
  if (onDestroy) onDestroy(this);
}

void DynamicPipeline::setRenderer(RendererCore* t_core) {
  rendererCore = t_core;

  core.init(t_core);
}

void DynamicPipeline::onUse() {
  dma_channel_fast_waits(DMA_CHANNEL_VIF1);

  colorsCache = new Vec4[4];

  buffers = new DynPipBag[buffersCount];

  for (unsigned int i = 0; i < buffersCount; i++) {
    buffers[i].texture = nullptr;
    buffers[i].lighting = nullptr;
  }

  auto packetSize =
      buffersCount * 5.1F;  // 5.1 = packet2_get_qw_count / buffersCount

  core.allocateOnUse(static_cast<unsigned int>(packetSize));
  core.reinitVU1Programs();
}

void DynamicPipeline::onUseEnd() {
  delete[] colorsCache;

  for (unsigned int i = 0; i < buffersCount; i++) freeBuffer(&buffers[i]);
  delete[] buffers;

  core.deallocateOnUse();
}

void DynamicPipeline::onFrameEnd() {}

void DynamicPipeline::render(const DynamicMesh* mesh) { render(mesh, nullptr); }

void DynamicPipeline::render(const DynamicMesh* mesh,
                             const DynPipOptions& options) {
  render(mesh, &options);
}

void DynamicPipeline::render(const DynamicMesh* mesh,
                             const DynPipOptions* options) {
  bool optionsManuallyAllocated = false;

  if (!options) {
    options = new DynPipOptions();
    optionsManuallyAllocated = true;
  }

  auto model = mesh->getModelMatrix();
  auto* infoBag = getInfoBag(mesh, options, &model);
  PipelineDirLightsBag* dirLights = nullptr;

  if (options->frustumCulling == PipelineFrustumCulling_Simple) {
    auto* frameTo = mesh->frames[mesh->animation.getState().nextFrame];
    if (frameTo->bbox->frustumCheck(
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

  unsigned short bufferIndex = 0;

  setBuffersDefaultVars(buffers, mesh, infoBag);
  core.begin(infoBag);

  for (unsigned int i = 0; i < mesh->materials.size(); i++) {
    auto* material = mesh->materials[i];

    auto* frameFrom = material->frames[mesh->animation.getState().currentFrame];
    auto* frameTo = material->frames[mesh->animation.getState().nextFrame];

    auto partSize = core.getMaxVertCountByParams(
        options && options->lighting, material->textureName.has_value());

    unsigned int partsCount =
        ceil(frameFrom->count / static_cast<float>(partSize));

    auto* colorBag = getColorBag(material);

    setBuffersColorBag(buffers, colorBag);
    unsigned char isPartInitialized = false;

    Texture* texture = nullptr;

    if (material->textureName.has_value()) {
      texture =
          rendererCore->texture.repository.getByMeshMaterialId(material->id);

      TYRA_ASSERT(texture, "Texture for material: ", material->name,
                  "Id: ", material->id,
                  "Was not found in texture repository! Did you forget to add "
                  "texture or disable texture coords loading in mesh?");
    }

    for (unsigned int k = 0; k < partsCount; k++) {
      auto& buffer = buffers[bufferIndex];

      freeBuffer(&buffer);
      buffer.count =
          k == partsCount - 1 ? frameFrom->count - k * partSize : partSize;

      unsigned int startIndex = k * partSize;

      addVertices(frameFrom, frameTo, &buffer, startIndex);

      buffer.texture = getTextureBag(texture, frameFrom, frameTo, startIndex);
      buffer.lighting = getLightingBag(frameFrom, frameTo, &model, options,
                                       dirLights, startIndex);

      if (!isPartInitialized) {
        core.sendObjectDataToVU1(&buffer);
        isPartInitialized = true;
      }

      setBuffer(buffers, &buffer, &bufferIndex);
    }

    sendRestOfBuffers(buffers, bufferIndex);
    bufferIndex = 0;
    core.begin(infoBag);

    delete colorBag;
  }

  if (dirLights) {
    delete dirLights;
  }

  delete infoBag;

  if (optionsManuallyAllocated) delete options;
}

void DynamicPipeline::setBuffer(DynPipBag* buffers, DynPipBag* buffer,
                                unsigned short* bufferIndex) {
  auto isEndOf1stDBuffer = *bufferIndex == halfBuffersCount - 1;
  auto isEndOf2ndDBuffer = *bufferIndex == buffersCount - 1;

  if (isEndOf1stDBuffer || isEndOf2ndDBuffer) {
    unsigned int offset = isEndOf1stDBuffer ? 0 : halfBuffersCount;

    DynPipBag** sendBuffers = new DynPipBag*[halfBuffersCount];

    for (unsigned int i = 0; i < halfBuffersCount; i++)
      sendBuffers[i] = &buffers[offset + i];

    core.render(sendBuffers, halfBuffersCount);

    delete[] sendBuffers;
  }

  if (isEndOf2ndDBuffer)
    *bufferIndex = 0;
  else
    *bufferIndex += 1;
}

void DynamicPipeline::sendRestOfBuffers(DynPipBag* buffers,
                                        const unsigned short& bufferIndex) {
  auto isEndOf1stDBuffer = bufferIndex <= halfBuffersCount - 1;

  unsigned int offset = isEndOf1stDBuffer ? 0 : halfBuffersCount;
  unsigned int size = bufferIndex - offset;

  if (size <= 0) return;

  DynPipBag** sendBuffers = new DynPipBag*[size];
  for (unsigned int i = 0; i < size; i++) {
    sendBuffers[i] = &buffers[offset + i];
  }

  core.render(sendBuffers, size);

  delete[] sendBuffers;
}

void DynamicPipeline::setBuffersDefaultVars(DynPipBag* buffers,
                                            const DynamicMesh* mesh,
                                            DynPipInfoBag* infoBag) {
  for (unsigned int i = 0; i < buffersCount; i++) {
    buffers[i].info = infoBag;
    buffers[i].interpolation = mesh->animation.getState().interpolation;
  }
}

void DynamicPipeline::setBuffersColorBag(DynPipBag* buffers,
                                         DynPipColorBag* colorBag) {
  for (unsigned int i = 0; i < buffersCount; i++) {
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

DynPipInfoBag* DynamicPipeline::getInfoBag(const DynamicMesh* mesh,
                                           const DynPipOptions* options,
                                           M4x4* model) const {
  auto* result = new DynPipInfoBag();

  result->antiAliasingEnabled = options->antiAliasingEnabled;
  result->blendingEnabled = options->blendingEnabled;
  result->shadingType = options->shadingType;
  result->textureMappingType = options->textureMappingType;
  result->transformationType = options->transformationType;
  result->zTestType = options->zTestType;
  result->frustumCulling =
      options->frustumCulling == PipelineFrustumCulling_Precise
          ? PipelineInfoBagFrustumCulling_Precise
          : PipelineInfoBagFrustumCulling_None;
  result->model = model;

  return result;
}

void DynamicPipeline::addVertices(const MeshMaterialFrame* materialFrameFrom,
                                  const MeshMaterialFrame* materialFrameTo,
                                  DynPipBag* bag,
                                  const unsigned int& startIndex) const {
  bag->verticesFrom = &materialFrameFrom->vertices[startIndex];
  bag->verticesTo = &materialFrameTo->vertices[startIndex];
}

DynPipColorBag* DynamicPipeline::getColorBag(
    const MeshMaterial* material) const {
  auto* result = new DynPipColorBag();
  result->single = &material->ambient;
  return result;
}

DynPipTextureBag* DynamicPipeline::getTextureBag(
    Texture* texture, const MeshMaterialFrame* materialFrameFrom,
    const MeshMaterialFrame* materialFrameTo, const unsigned int& startIndex) {
  if (!materialFrameFrom->textureCoords || texture == nullptr) return nullptr;

  auto* result = new DynPipTextureBag();

  result->texture = texture;

  result->coordinatesFrom = &materialFrameFrom->textureCoords[startIndex];
  result->coordinatesTo = &materialFrameTo->textureCoords[startIndex];

  return result;
}

DynPipLightingBag* DynamicPipeline::getLightingBag(
    const MeshMaterialFrame* materialFrameFrom,
    const MeshMaterialFrame* materialFrameTo, M4x4* model,
    const DynPipOptions* options, PipelineDirLightsBag* dirLightsBag,
    const unsigned int& startIndex) const {
  if (!materialFrameFrom->normals || options == nullptr ||
      options->lighting == nullptr)
    return nullptr;

  auto* result = new DynPipLightingBag();

  result->lightMatrix = model;
  result->dirLights = dirLightsBag;

  result->normalsFrom = &materialFrameFrom->normals[startIndex];
  result->normalsTo = &materialFrameTo->normals[startIndex];

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
