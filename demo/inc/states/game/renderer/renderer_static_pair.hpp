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

#include "renderer/3d/pipeline/static/static_pipeline.hpp"
#include "renderer/3d/mesh/static/static_mesh.hpp"

using Tyra::StaPipOptions;
using Tyra::StaticMesh;

namespace Demo {

struct RendererStaticPair {
  StaticMesh* mesh;
  StaPipOptions* options;
};

}  // namespace Demo
