/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/static/static_pipeline.hpp"
#include "debug/debug.hpp"
#include "renderer/3d/pipeline/static/core/bag/stapip_info_bag.hpp"

namespace Tyra {

StaticPipeline::StaticPipeline() {}

StaticPipeline::~StaticPipeline() {
  if (onDestroy) onDestroy(this);
}

void StaticPipeline::setRenderer(RendererCore* t_core) {
  rendererCore = t_core;
  core.init(t_core);
}

void StaticPipeline::onUse() {
  colorsCache = new Vec4[4];
  core.allocateOnUse();
  core.reinitVU1Programs();
}

void StaticPipeline::onUseEnd() {
  delete[] colorsCache;

  core.deallocateOnUse();
}

void StaticPipeline::render(StaticMesh* mesh, const StaPipOptions* options) {
  bool optionsManuallyAllocated = false;

  if (!options) {
    options = new StaPipOptions();
    optionsManuallyAllocated = true;
  }

  auto model = mesh->getModelMatrix();
  auto* infoBag = getInfoBag(mesh, options, &model);

  TYRA_ASSERT(
      !(options->frustumCulling != PipelineFrustumCulling_Precise &&
        infoBag->fullClipChecks == true),
      "Full clip checks are only supported with frustum culling == Precise!");
  TYRA_ASSERT(options->transformationType == TyraMVP ||
                  (!options->fullClipChecks &&
                   options->frustumCulling == PipelineFrustumCulling_None),
              "Please disable clip checks and frustum culling if not using MVP "
              "matrix!");

  if (options->frustumCulling == PipelineFrustumCulling_Simple) {
    auto* frame = mesh->frame;
    if (frame->bbox->isInFrustum(
            rendererCore->renderer3D.frustumPlanes.getAll(), model) ==
        CoreBBoxFrustum::OUTSIDE_FRUSTUM) {
      return;
    }
  }

  if (options && options->lighting) setLightingColorsCache(options->lighting);

  for (u32 i = 0; i < mesh->materials.size(); i++) {
    auto* material = mesh->materials[i];
    auto* materialFrame = material->frames[0];
    StaPipBag bag;
    addVertices(materialFrame, &bag);
    bag.info = infoBag;
    bag.color = getColorBag(material, materialFrame);
    bag.texture = getTextureBag(material, materialFrame);
    bag.lighting = getLightingBag(materialFrame, &model, options);
    core.render(&bag);

    deallocDrawBags(&bag, material);
  }
  delete infoBag;

  if (optionsManuallyAllocated) delete options;
}

void StaticPipeline::addVertices(MeshMaterialFrame* materialFrame,
                                 StaPipBag* bag) const {
  bag->count = materialFrame->count;
  bag->vertices = materialFrame->vertices;
}

StaPipInfoBag* StaticPipeline::getInfoBag(StaticMesh* mesh,
                                          const StaPipOptions* options,
                                          M4x4* model) const {
  auto* result = new StaPipInfoBag();

  result->antiAliasingEnabled = options->antiAliasingEnabled;
  result->blendingEnabled = options->blendingEnabled;
  result->shadingType = options->shadingType;
  result->textureMappingType = options->textureMappingType;
  result->transformationType = options->transformationType;
  result->frustumCulling =
      options->frustumCulling == PipelineFrustumCulling_Precise
          ? PipelineInfoBagFrustumCulling_Precise
          : PipelineInfoBagFrustumCulling_None;
  result->fullClipChecks = options->fullClipChecks;
  result->zTestType = options->zTestType;
  result->model = model;

  return result;
}

StaPipColorBag* StaticPipeline::getColorBag(
    MeshMaterial* material, MeshMaterialFrame* materialFrame) const {
  auto* result = new StaPipColorBag();

  if (material->lightmapFlag) {
    result->single = &material->ambient;
  } else {
    result->many = materialFrame->colors;
  }

  return result;
}

StaPipTextureBag* StaticPipeline::getTextureBag(
    MeshMaterial* material, MeshMaterialFrame* materialFrame) {
  if (!materialFrame->textureCoords) return nullptr;

  auto* result = new StaPipTextureBag();

  result->texture =
      rendererCore->texture.repository.getBySpriteOrMesh(material->id);

  TYRA_ASSERT(
      result->texture, "Texture for material: ", material->name,
      "Id: ", material->id,
      "Was not found in texture repository! Did you forget to add texture?");

  result->coordinates = materialFrame->textureCoords;

  return result;
}

StaPipLightingBag* StaticPipeline::getLightingBag(
    MeshMaterialFrame* materialFrame, M4x4* model,
    const StaPipOptions* options) const {
  if (!materialFrame->normals || options == nullptr ||
      options->lighting == nullptr)
    return nullptr;

  auto* result = new StaPipLightingBag();

  // TODO: Make it once per rendering, very redundant
  auto* dirLightsBag = new PipelineDirLightsBag(true);
  result->dirLights = dirLightsBag;

  result->lightMatrix = model;

  result->dirLights->setLightsManually(
      colorsCache, options->lighting->directionalDirections);

  result->normals = materialFrame->normals;

  return result;
}

void StaticPipeline::setLightingColorsCache(
    PipelineLightingOptions* lightingOptions) {
  for (int i = 0; i < 3; i++) {
    colorsCache[i] =
        reinterpret_cast<Vec4&>(lightingOptions->directionalColors[i]);
  }
  colorsCache[3] = reinterpret_cast<Vec4&>(*lightingOptions->ambientColor);
}

void StaticPipeline::deallocDrawBags(StaPipBag* bag,
                                     MeshMaterial* material) const {
  if (bag->texture) {
    delete bag->texture;
  }

  if (bag->lighting) {
    delete bag->lighting->dirLights;  // TODO
    delete bag->lighting;
  }

  delete bag->color;
}

}  // namespace Tyra
