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
#include "./stapip_options.hpp"
#include "renderer/core/renderer_core.hpp"
#include "renderer/3d/mesh/dynamic/dynamic_mesh.hpp"
#include "./core/static_pipeline_core.hpp"

namespace Tyra {

class StaticPipeline : public Renderer3DPipeline {
 public:
  StaticPipeline();
  ~StaticPipeline();

  void init(RendererCore* core);

  void onUse();

  /**
   * Render 3D data via "meshes".
   * This render() method is a bridge to core.render() method.
   */
  void render(DynamicMesh* mesh, const StaPipOptions* options = nullptr);

 private:
  StaPipelineCore core;
  RendererCore* rendererCore;
  Vec4* colorsCache;

  void addVertices(DynamicMesh* mesh, MeshMaterial* material, StaPipBag* bag,
                   MeshFrame* frameFrom, MeshFrame* frameTo) const;
  PipelineInfoBag* getInfoBag(DynamicMesh* mesh, const StaPipOptions* options,
                              M4x4* model) const;
  StaPipColorBag* getColorBag(DynamicMesh* mesh, MeshMaterial* material,
                              MeshFrame* frameFrom, MeshFrame* frameTo) const;
  StaPipTextureBag* getTextureBag(DynamicMesh* mesh, MeshMaterial* material,
                                  MeshFrame* frameFrom, MeshFrame* frameTo);
  PipelineLightingBag* getLightingBag(DynamicMesh* mesh, MeshMaterial* material,
                                      M4x4* model, MeshFrame* frameFrom,
                                      MeshFrame* frameTo,
                                      const StaPipOptions* options) const;
  void deallocDrawBags(StaPipBag* bag, MeshMaterial* material) const;

  void setLightingColorsCache(StaPipLightingOptions* lightingOptions);
};

}  // namespace Tyra
