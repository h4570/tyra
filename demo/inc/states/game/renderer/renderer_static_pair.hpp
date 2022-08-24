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

using Tyra::StaPipOptions;
using Tyra::StaticMesh;

namespace Demo {

struct RendererStaticPair {
  StaticMesh* mesh;
  StaPipOptions* options;
};

}  // namespace Demo
