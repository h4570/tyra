/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <tamtypes.h>
#include "../renderer_3d_pipeline.hpp"
#include "../shared/pipeline_lighting_options.hpp"
#include "renderer/core/renderer_core.hpp"
#include "renderer/3d/mesh/static/static_mesh.hpp"
#include "./core/stapip_core.hpp"
#include "./stapip_options.hpp"

namespace Tyra {

/**
 * Pipeline for static models (StaticMesh).
 * Supports:
 * - Full "against each plane" clipping and simple PS2 clipping
 * (fullClipChecks),
 * - Force enabled precise frustum culling (checks parts of given mesh)
 * - Modes: color(s), texture+color(s), dir lights, texture + dir lights
 */
class StaticPipeline : public Renderer3DPipeline {
 public:
  StaticPipeline();
  ~StaticPipeline();

  StaPipCore core;

  void init(RendererCore* core);

  void onUse();

  /**
   * Render static model
   * This render() method is a bridge to core.render() method.
   */
  void render(StaticMesh* mesh, const StaPipOptions* options = nullptr);

 private:
  RendererCore* rendererCore;
  Vec4* colorsCache;

  void addVertices(MeshMaterialFrame* materialFrame, StaPipBag* bag) const;

  PipelineInfoBag* getInfoBag(StaticMesh* mesh, const StaPipOptions* options,
                              M4x4* model) const;

  StaPipColorBag* getColorBag(MeshMaterial* material,
                              MeshMaterialFrame* materialFrame) const;

  StaPipTextureBag* getTextureBag(MeshMaterial* material,
                                  MeshMaterialFrame* materialFrame);

  StaPipLightingBag* getLightingBag(MeshMaterialFrame* materialFrame,
                                    M4x4* model,
                                    const StaPipOptions* options) const;

  void deallocDrawBags(StaPipBag* bag, MeshMaterial* material) const;

  void setLightingColorsCache(PipelineLightingOptions* lightingOptions);
};

}  // namespace Tyra
