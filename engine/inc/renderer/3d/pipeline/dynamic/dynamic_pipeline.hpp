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
#include "./dynpip_options.hpp"
#include "./core/dynpip_core.hpp"
#include "renderer/core/renderer_core.hpp"
#include "renderer/3d/mesh/dynamic/dynamic_mesh.hpp"

namespace Tyra {

class DynamicPipeline : public Renderer3DPipeline {
 public:
  DynamicPipeline();
  ~DynamicPipeline();

  DynPipCore core;

  void init(RendererCore* core);

  void onUse();

  /**
   * Render 3D via "meshes".
   * This render() method is a bridge to core.render() method.
   */
  void render(DynamicMesh* mesh, const DynPipOptions* options = nullptr);

 private:
  RendererCore* rendererCore;
  Vec4* colorsCache;

  void addVertices(DynamicMesh* mesh, MeshMaterial* material, DynPipBag* bag,
                   MeshFrame* frameFrom, MeshFrame* frameTo) const;
  PipelineInfoBag* getInfoBag(DynamicMesh* mesh, const DynPipOptions* options,
                              M4x4* model) const;
  DynPipColorBag* getColorBag(DynamicMesh* mesh, MeshMaterial* material,
                              MeshFrame* frameFrom, MeshFrame* frameTo) const;
  DynPipTextureBag* getTextureBag(DynamicMesh* mesh, MeshMaterial* material,
                                  MeshFrame* frameFrom, MeshFrame* frameTo);
  DynPipLightingBag* getLightingBag(DynamicMesh* mesh, MeshMaterial* material,
                                    M4x4* model, MeshFrame* frameFrom,
                                    MeshFrame* frameTo,
                                    const DynPipOptions* options) const;
  void deallocDrawBags(DynPipBag* bag, MeshMaterial* material) const;

  void setLightingColorsCache(PipelineLightingOptions* lightingOptions);
};

}  // namespace Tyra
