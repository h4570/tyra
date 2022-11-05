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
#include "./bag/dynpip_bag.hpp"
#include "renderer/core/renderer_core.hpp"
#include "./dynpip_programs_repository.hpp"
#include "./dynpip_renderer.hpp"

namespace Tyra {

class DynPipCore {
 public:
  DynPipCore();
  ~DynPipCore();

  DynPipProgramsRepository repository;

  void init(RendererCore* t_core);

  /** Force starting VU1 program instead of continueing */
  void begin(PipelineInfoBag* bag);

  /**
   * Send model matrix, lighting data and other
   * repetitve stuff to VU1
   */
  void sendObjectDataToVU1(DynPipBag* data);

  /** Render 3D via "bags" */
  void render(DynPipBag** bags, const u32& count);

  /** Get max vert count of VU1 qbuffer (for optimizations) */
  u32 getMaxVertCountByParams(const bool& isLightingEnabled,
                              const bool& isTextureEnabled);

  /**
   * - Uploads standard VU1 programs.
   * - Sends static "Tyra Renderer3D" VU1 data.
   * - Sets double buffers exactly for "Tyra Renderer3D"
   * Should be called if VU1 was used by your non standard programs.
   */
  void reinitVU1Programs();

  void allocateOnUse(const u32& t_packetSize) {
    qbufferRenderer.allocateOnUse(t_packetSize);
  }
  void deallocateOnUse() { qbufferRenderer.deallocateOnUse(); }

 private:
  void setPrim();
  void setLod();

  prim_t prim;
  lod_t lod;

  M4x4 mvp;
  RendererCore* rendererCore;
  DynPipRenderer qbufferRenderer;
};

}  // namespace Tyra
