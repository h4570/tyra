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
  void clear() { qbufferRenderer.clearLastProgramName(); }

  /**
   * Send model matrix, lighting data and other
   * repetitve stuff to VU1
   */
  void initParts(DynPipBag* data);

  // TODO: TEST!
  void renderTEST(DynPipBag** bags, const u32& count);

  /** Render 3D via "bags" */
  void renderPart(DynPipBag* data, const bool& frustumCull = true);

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

 private:
  M4x4 mvp;
  RendererCore* rendererCore;
  DynPipRenderer qbufferRenderer;
};

}  // namespace Tyra
