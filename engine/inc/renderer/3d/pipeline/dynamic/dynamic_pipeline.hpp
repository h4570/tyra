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
#include "./core/dynpip_options.hpp"
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
};

}  // namespace Tyra
