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
#include "./stdpip_options.hpp"
#include "renderer/core/renderer_core.hpp"
#include "renderer/3d/mesh/mesh.hpp"
#include "./core/std_pipeline_core.hpp"

namespace Tyra {

class StdPipeline : public Renderer3DPipeline {
 public:
  StdPipeline();
  ~StdPipeline();

  void init(RendererCore* core);

  void onUse();

  /**
   * Render 3D data via "meshes".
   * This render() method is a bridge to core.renderer3D.render() method.
   */
  void render(Mesh* mesh, const StdpipOptions* options = nullptr);

 private:
  StdPipelineCore core;
  RendererCore* rendererCore;
  Vec4* colorsCache;

  void addVertices(Mesh* mesh, MeshMaterial* material, StdpipBag* bag,
                   MeshFrame* frameFrom, MeshFrame* frameTo) const;
  StdpipInfoBag* getInfoBag(Mesh* mesh, const StdpipOptions* options,
                            M4x4* model) const;
  StdpipColorBag* getColorBag(Mesh* mesh, MeshMaterial* material,
                              MeshFrame* frameFrom, MeshFrame* frameTo) const;
  StdpipTextureBag* getTextureBag(Mesh* mesh, MeshMaterial* material,
                                  MeshFrame* frameFrom, MeshFrame* frameTo);
  StdpipLightingBag* getLightingBag(Mesh* mesh, MeshMaterial* material,
                                    M4x4* model, MeshFrame* frameFrom,
                                    MeshFrame* frameTo,
                                    const StdpipOptions* options) const;
  void deallocDrawBags(StdpipBag* bag, MeshMaterial* material) const;

  void setLightingColorsCache(StdpipLightingOptions* lightingOptions);
};

}  // namespace Tyra
