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
#include "./dynpip_options.hpp"
#include "./core/dynpip_core.hpp"
#include "shared/renderer/core/renderer_core.hpp"
#include "shared/renderer/3d/mesh/dynamic/dynamic_mesh.hpp"

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

  static const unsigned int buffersCount;

  DynPipCore core;

  void setRenderer(RendererCore* core);

  void onUse();

  void onFrameEnd();

  void onUseEnd();

  /**
   * Render dynamic model.
   * This render() method is a bridge to core.render() method.
   */
  void render(const DynamicMesh* mesh);
  void render(const DynamicMesh* mesh, const DynPipOptions& options);
  void render(const DynamicMesh* mesh, const DynPipOptions* options);

 private:
  RendererCore* rendererCore;
  Vec4* colorsCache;
  DynPipBag* buffers;
  static const unsigned int halfBuffersCount;

  void setBuffer(DynPipBag* buffers, DynPipBag* buffer,
                 unsigned short* bufferIndex);

  void sendRestOfBuffers(DynPipBag* buffers, const unsigned short& bufferIndex);

  void addVertices(const MeshMaterialFrame* materialFrameFrom,
                   const MeshMaterialFrame* materialFrameTo, DynPipBag* bag,
                   const unsigned int& startIndex) const;

  DynPipInfoBag* getInfoBag(const DynamicMesh* mesh,
                            const DynPipOptions* options, M4x4* model) const;

  DynPipColorBag* getColorBag(const MeshMaterial* material) const;

  DynPipTextureBag* getTextureBag(Texture* texture,
                                  const MeshMaterialFrame* materialFrameFrom,
                                  const MeshMaterialFrame* materialFrameTo,
                                  const unsigned int& startIndex);

  DynPipLightingBag* getLightingBag(const MeshMaterialFrame* materialFrameFrom,
                                    const MeshMaterialFrame* materialFrameTo,
                                    M4x4* model, const DynPipOptions* options,
                                    PipelineDirLightsBag* dirLightsBag,
                                    const unsigned int& startIndex) const;

  void setLightingColorsCache(PipelineLightingOptions* lightingOptions);
  void freeBuffer(DynPipBag* bag);
  void setBuffersDefaultVars(DynPipBag* buffers, const DynamicMesh* mesh,
                             DynPipInfoBag* infoBag);
  void setBuffersColorBag(DynPipBag* buffers, DynPipColorBag* colorBag);
};

}  // namespace Tyra
