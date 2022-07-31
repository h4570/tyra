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

#include "renderer/3d/pipeline/dynamic/dynamic_pipeline.hpp"
#include "renderer/3d/mesh/dynamic/dynamic_mesh.hpp"

using Tyra::DynamicMesh;
using Tyra::DynPipOptions;

namespace Demo {

struct RendererDynamicPair {
  DynamicMesh* mesh;
  DynPipOptions* options;
};

}  // namespace Demo
