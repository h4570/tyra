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

#include <tamtypes.h>
#include "../renderer_3d_pipeline.hpp"
#include "./dynpip_options.hpp"
#include "./core/dynpip_core.hpp"
#include "renderer/core/renderer_core.hpp"
#include "renderer/3d/mesh/dynamic/dynamic_mesh.hpp"

namespace Tyra {

/**
 * Pipeline for animated models (DynamicMesh).
 * Supports:
 * - Simple PS2 clipping (culling)
 * - Frustum culling
 * - Modes: color, texture+color, dir lights, texture + dir lights
 */
class DynamicPipeline : public Renderer3DPipeline {
 public:
  DynamicPipeline();
  ~DynamicPipeline();

  static const u32 buffersCount;

  DynPipCore core;

  void setRenderer(RendererCore* core);

  void onUse();

  void onUseEnd();

  /**
   * Render dynamic model.
   * This render() method is a bridge to core.render() method.
   */
  void render(DynamicMesh* mesh, const DynPipOptions* options = nullptr);

 private:
  RendererCore* rendererCore;
  Vec4* colorsCache;
  DynPipBag* buffers;
  static const u32 halfBuffersCount;

  void setBuffer(DynPipBag* buffers, DynPipBag* buffer, u16* bufferIndex);

  void sendRestOfBuffers(DynPipBag* buffers, u16* bufferIndex);

  void addVertices(MeshMaterialFrame* materialFrameFrom,
                   MeshMaterialFrame* materialFrameTo, DynPipBag* bag,
                   const u32& startIndex) const;

  DynPipInfoBag* getInfoBag(DynamicMesh* mesh, const DynPipOptions* options,
                            M4x4* model) const;

  DynPipColorBag* getColorBag(MeshMaterial* material) const;

  DynPipTextureBag* getTextureBag(Texture* texture,
                                  MeshMaterialFrame* materialFrameFrom,
                                  MeshMaterialFrame* materialFrameTo,
                                  const u32& startIndex);

  DynPipLightingBag* getLightingBag(MeshMaterialFrame* materialFrameFrom,
                                    MeshMaterialFrame* materialFrameTo,
                                    M4x4* model, const DynPipOptions* options,
                                    PipelineDirLightsBag* dirLightsBag,
                                    const u32& startIndex) const;

  void setLightingColorsCache(PipelineLightingOptions* lightingOptions);
  void freeBuffer(DynPipBag* bag);
  void setBuffersDefaultVars(DynPipBag* buffers, DynamicMesh* mesh,
                             DynPipInfoBag* infoBag);
  void setBuffersColorBag(DynPipBag* buffers, DynPipColorBag* colorBag);
};

}  // namespace Tyra
