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

class Stapipeline : public Renderer3DPipeline {
 public:
  Stapipeline();
  ~Stapipeline();

  void init(RendererCore* core);

  void onUse();

  /**
   * Render 3D data via "meshes".
   * This render() method is a bridge to core.renderer3D.render() method.
   */
  void render(DynamicMesh* mesh, const StapipOptions* options = nullptr);

 private:
  StapipelineCore core;
  RendererCore* rendererCore;
  Vec4* colorsCache;

  void addVertices(DynamicMesh* mesh, MeshMaterial* material, StapipBag* bag,
                   MeshFrame* frameFrom, MeshFrame* frameTo) const;
  StapipInfoBag* getInfoBag(DynamicMesh* mesh, const StapipOptions* options,
                            M4x4* model) const;
  StapipColorBag* getColorBag(DynamicMesh* mesh, MeshMaterial* material,
                              MeshFrame* frameFrom, MeshFrame* frameTo) const;
  StapipTextureBag* getTextureBag(DynamicMesh* mesh, MeshMaterial* material,
                                  MeshFrame* frameFrom, MeshFrame* frameTo);
  StapipLightingBag* getLightingBag(DynamicMesh* mesh, MeshMaterial* material,
                                    M4x4* model, MeshFrame* frameFrom,
                                    MeshFrame* frameTo,
                                    const StapipOptions* options) const;
  void deallocDrawBags(StapipBag* bag, MeshMaterial* material) const;

  void setLightingColorsCache(StapipLightingOptions* lightingOptions);
};

}  // namespace Tyra
