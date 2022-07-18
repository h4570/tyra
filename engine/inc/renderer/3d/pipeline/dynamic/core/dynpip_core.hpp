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

namespace Tyra {

class DynPipCore {
 public:
  DynPipCore();
  ~DynPipCore();

  void init(RendererCore* t_core);

  /** Render 3D via "bags" */
  void render(DynPipBag* data);

  /**
   * - Uploads standard VU1 programs.
   * - Sends static "Tyra Renderer3D" VU1 data.
   * - Sets double buffers exactly for "Tyra Renderer3D"
   * Should be called if VU1 was used by your non standard programs.
   */
  void reinitVU1Programs();

 private:
  RendererCore* rendererCore;
};

}  // namespace Tyra
