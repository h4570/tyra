/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "../renderer_3d_pipeline.hpp"
#include "../shared/pipeline_lighting_options.hpp"
#include "shared/renderer/core/renderer_core.hpp"
#include "shared/renderer/3d/mesh/static/static_mesh.hpp"
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

  void setRenderer(RendererCore* core);

  void onUse();
  void onUseEnd();
  void onFrameEnd();

  /**
   * Render static model
   * This render() method is a bridge to core.render() method.
   */
  void render(const StaticMesh* mesh);
  void render(const StaticMesh* mesh, const StaPipOptions& options);
  void render(const StaticMesh* mesh, const StaPipOptions* options);

 private:
  RendererCore* rendererCore;
  Vec4* colorsCache;

  void addVertices(const MeshMaterialFrame* materialFrame,
                   StaPipBag* bag) const;

  StaPipInfoBag* getInfoBag(const StaticMesh* mesh,
                            const StaPipOptions* options, M4x4* model) const;

  StaPipColorBag* getColorBag(const MeshMaterial* material,
                              const MeshMaterialFrame* materialFrame) const;

  StaPipTextureBag* getTextureBag(const MeshMaterial* material,
                                  const MeshMaterialFrame* materialFrame);

  StaPipLightingBag* getLightingBag(const MeshMaterialFrame* materialFrame,
                                    PipelineDirLightsBag* dirLightsBag,
                                    M4x4* model,
                                    const StaPipOptions* options) const;

  void deallocDrawBags(StaPipBag* bag, const MeshMaterial* material) const;

  void setLightingColorsCache(PipelineLightingOptions* lightingOptions);
};

}  // namespace Tyra
