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

#include <tyra>

using Tyra::DynamicMesh;
using Tyra::DynPipOptions;

namespace Demo {

struct RendererDynamicPair {
  DynamicMesh* mesh;
  DynPipOptions* options;
};

}  // namespace Demo
