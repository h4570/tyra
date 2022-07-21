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

namespace Tyra {

StaticPipeline::StaticPipeline() { colorsCache = new Vec4[4]; }

StaticPipeline::~StaticPipeline() { delete[] colorsCache; }

void StaticPipeline::init(RendererCore* t_core) {
  rendererCore = t_core;
  core.init(t_core);
}

void StaticPipeline::onUse() { core.reinitVU1Programs(); }

void StaticPipeline::render(DynamicMesh* mesh, const StaPipOptions* options) {
  auto model = mesh->getModelMatrix();
  auto* infoBag = getInfoBag(mesh, options, &model);

  if (options && options->lighting) setLightingColorsCache(options->lighting);

  for (u32 i = 0; i < mesh->getMaterialsCount(); i++) {
    auto* material = mesh->getMaterial(i);
    auto* materialFrame = material->getFrame(0);

    // 2x bufory[maxVertCount*2] -> pętla po mniejszych częściach i czestsze
    // rendery

    // TODO: Double buffering in future
    // auto maxVertCount = core->renderer3D.getMaxVertCountByParams(
    //     material->isSingleColorActivated(), material->getNormalFaces(),
    //     material->getTextureCoordFaces());

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
}

void StaticPipeline::addVertices(MeshMaterialFrame* materialFrame,
                                 StaPipBag* bag) const {
  bag->count = materialFrame->getVertexCount();
  bag->vertices = materialFrame->getVertices();
}

PipelineInfoBag* StaticPipeline::getInfoBag(DynamicMesh* mesh,
                                            const StaPipOptions* options,
                                            M4x4* model) const {
  auto* result = new PipelineInfoBag();

  if (options) {
    result->antiAliasingEnabled = options->antiAliasingEnabled;
    result->blendingEnabled = options->blendingEnabled;
    result->shadingType = options->shadingType;
    result->noFullClipChecks = options->noFullClipChecks;
  } else {
    result->antiAliasingEnabled = false;
    result->blendingEnabled = true;
    result->shadingType = TyraShadingFlat;
    result->noFullClipChecks = false;
  }

  result->model = model;

  return result;
}

StaPipColorBag* StaticPipeline::getColorBag(
    MeshMaterial* material, MeshMaterialFrame* materialFrame) const {
  auto* result = new StaPipColorBag();

  if (material->isSingleColorActivated()) {
    result->single = &material->color;
  } else {
    result->many = materialFrame->getColors();
  }

  return result;
}

StaPipTextureBag* StaticPipeline::getTextureBag(
    MeshMaterial* material, MeshMaterialFrame* materialFrame) {
  if (!materialFrame->getTextureCoords()) return nullptr;

  auto* result = new StaPipTextureBag();

  result->texture =
      rendererCore->texture.repository.getBySpriteOrMesh(material->getId());
  TYRA_ASSERT(result->texture, "Texture for material id: ", material->getId(),
              "was not found in texture repository!");

  result->coordinates = materialFrame->getTextureCoords();

  return result;
}

StaPipLightingBag* StaticPipeline::getLightingBag(
    MeshMaterialFrame* materialFrame, M4x4* model,
    const StaPipOptions* options) const {
  if (!materialFrame->getNormals() || options == nullptr ||
      options->lighting == nullptr)
    return nullptr;

  auto* result = new StaPipLightingBag();
  auto* dirLightsBag = new PipelineDirLightsBag(true);  // TODO: 1 dir
  //   lights per obiekt, dealokowac recznie
  result->dirLights = dirLightsBag;

  result->lightMatrix = model;

  result->dirLights->setLightsManually(
      colorsCache, options->lighting->directionalDirections);

  result->normals = materialFrame->getNormals();

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
