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

Stapipeline::Stapipeline() { colorsCache = new Vec4[4]; }

Stapipeline::~Stapipeline() { delete[] colorsCache; }

void Stapipeline::init(RendererCore* t_core) {
  rendererCore = t_core;
  core.init(t_core);
}

void Stapipeline::onUse() { core.reinitStandardVU1Programs(); }

void Stapipeline::render(DynamicMesh* mesh, const StapipOptions* options) {
  auto model = mesh->getModelMatrix();

  DynamicMeshFrame* frameFrom =
      mesh->getFramesCount() > 0
          ? mesh->getFrame(mesh->getCurrentAnimationFrame())
          : mesh->getFrame(0);

  DynamicMeshFrame* frameTo =
      mesh->getFramesCount() > 0 ? mesh->getFrame(mesh->getNextAnimationFrame())
                                 : nullptr;

  auto* infoBag = getInfoBag(mesh, options, &model);

  if (options->lighting) setLightingColorsCache(options->lighting);

  for (u32 i = 0; i < mesh->getMaterialsCount(); i++) {
    auto* material = mesh->getMaterial(i);

    // 2x bufory[maxVertCount*2] -> pętla po mniejszych częściach i czestsze
    // rendery

    // TODO: Double buffering in future
    // auto maxVertCount = core->renderer3D.getMaxVertCountByParams(
    //     material->isSingleColorActivated(), material->getNormalFaces(),
    //     material->getTextureCoordFaces());

    StapipBag bag;
    addVertices(mesh, material, &bag, frameFrom, frameTo);
    bag.info = infoBag;
    bag.color = getColorBag(mesh, material, frameFrom, frameTo);
    bag.texture = getTextureBag(mesh, material, frameFrom, frameTo);
    bag.lighting =
        getLightingBag(mesh, material, &model, frameFrom, frameTo, options);

    core.render(&bag);

    deallocDrawBags(&bag, material);
  }

  delete infoBag;
}

void Stapipeline::addVertices(DynamicMesh* mesh, DynamicMeshMaterial* material,
                              StapipBag* bag, DynamicMeshFrame* frameFrom,
                              DynamicMeshFrame* frameTo) const {
  bag->count = material->getFacesCount();
  bag->vertices = new Vec4[bag->count];

  for (u32 i = 0; i < bag->count; i++) {
    auto& face = material->getVertexFaces()[i];
    if (frameTo == nullptr) {
      bag->vertices[i] = frameFrom->getVertices()[face];
    } else {
      Vec4::setLerp(&bag->vertices[i], frameFrom->getVertices()[face],
                    frameTo->getVertices()[face],
                    mesh->getAnimState().interpolation);
    }
  }
}

StapipInfoBag* Stapipeline::getInfoBag(DynamicMesh* mesh,
                                       const StapipOptions* options,
                                       M4x4* model) const {
  auto* result = new StapipInfoBag();

  if (options) {
    result->antiAliasingEnabled = options->antiAliasingEnabled;
    result->blendingEnabled = options->blendingEnabled;
    result->shadingType = options->shadingType;
    result->noClipChecks = options->noClipChecks;
  } else {
    result->antiAliasingEnabled = false;
    result->blendingEnabled = true;
    result->shadingType = StapipShadingFlat;
    result->noClipChecks = true;
  }

  result->model = model;

  return result;
}

StapipColorBag* Stapipeline::getColorBag(DynamicMesh* mesh,
                                         DynamicMeshMaterial* material,
                                         DynamicMeshFrame* frameFrom,
                                         DynamicMeshFrame* frameTo) const {
  auto* result = new StapipColorBag();

  if (material->isSingleColorActivated()) {
    result->single = &material->singleColor;
  } else {
    result->many = new Color[material->getFacesCount()];

    for (u32 i = 0; i < material->getFacesCount(); i++) {
      auto& face = material->getColorFaces()[i];
      if (frameTo == nullptr) {
        result->many[i] = frameFrom->getColors()[face];
      } else {
        Vec4::setLerp(
            reinterpret_cast<Vec4*>(&result->many[i]),
            reinterpret_cast<const Vec4&>(frameFrom->getColors()[face]),
            reinterpret_cast<const Vec4&>(frameTo->getColors()[face]),
            mesh->getAnimState().interpolation);
      }
    }
  }

  return result;
}

StapipTextureBag* Stapipeline::getTextureBag(DynamicMesh* mesh,
                                             DynamicMeshMaterial* material,
                                             DynamicMeshFrame* frameFrom,
                                             DynamicMeshFrame* frameTo) {
  if (!material->getTextureCoordFaces()) return nullptr;

  auto* result = new StapipTextureBag();

  result->texture =
      rendererCore->texture.repository.getBySpriteOrMesh(material->getId());
  TYRA_ASSERT(result->texture, "Texture for material id: ", material->getId(),
              " was not found in texture repository!");

  result->coordinates = new Vec4[material->getFacesCount()];

  for (u32 i = 0; i < material->getFacesCount(); i++) {
    auto& face = material->getTextureCoordFaces()[i];
    if (frameTo == nullptr) {
      result->coordinates[i] = frameFrom->getTextureCoords()[face];
    } else {
      Vec4::setLerp(&result->coordinates[i],
                    frameFrom->getTextureCoords()[face],
                    frameTo->getTextureCoords()[face],
                    mesh->getAnimState().interpolation);
    }
  }

  return result;
}

StapipLightingBag* Stapipeline::getLightingBag(
    DynamicMesh* mesh, DynamicMeshMaterial* material, M4x4* model,
    DynamicMeshFrame* frameFrom, DynamicMeshFrame* frameTo,
    const StapipOptions* options) const {
  if (!material->getNormalFaces() || options == nullptr ||
      options->lighting == nullptr)
    return nullptr;

  auto* result = new StapipLightingBag(true);
  result->lightMatrix = model;

  result->setLightsManually(colorsCache,
                            options->lighting->directionalDirections);

  result->normals = new Vec4[material->getFacesCount()];

  for (u32 i = 0; i < material->getFacesCount(); i++) {
    auto& face = material->getNormalFaces()[i];
    if (frameTo == nullptr) {
      result->normals[i] = frameFrom->getNormals()[face];
    } else {
      Vec4::setLerp(&result->normals[i], frameFrom->getNormals()[face],
                    frameTo->getNormals()[face],
                    mesh->getAnimState().interpolation);
    }
  }

  return result;
}

void Stapipeline::setLightingColorsCache(
    StapipLightingOptions* lightingOptions) {
  for (int i = 0; i < 3; i++) {
    colorsCache[i] =
        reinterpret_cast<Vec4&>(lightingOptions->directionalColors[i]);
  }
  colorsCache[3] = reinterpret_cast<Vec4&>(*lightingOptions->ambientColor);
}

void Stapipeline::deallocDrawBags(StapipBag* bag,
                                  DynamicMeshMaterial* material) const {
  if (bag->color->many) {
    delete[] bag->color->many;
  }

  if (bag->texture) {
    delete[] bag->texture->coordinates;
    delete bag->texture;
  }

  if (bag->lighting) {
    delete[] bag->lighting->normals;
    delete bag->lighting;
  }

  delete[] bag->vertices;
  delete bag->color;
}

}  // namespace Tyra
