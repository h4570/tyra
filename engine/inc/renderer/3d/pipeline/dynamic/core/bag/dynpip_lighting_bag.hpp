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

#include "math/vec4.hpp"
#include "renderer/3d/pipeline/shared/bag/pipeline_dir_lights_bag.hpp"

namespace Tyra {

class DynPipLightingBag {
 public:
  DynPipLightingBag();
  ~DynPipLightingBag();

  /** Mandatory. Model matrix for lights. */
  M4x4* lightMatrix;

  /** Mandatory. Lighting normals per vertex. */
  Vec4* normalsFrom;
  Vec4* normalsTo;

  /** Mandatory. Directional lights */
  PipelineDirLightsBag* dirLights;
};

}  // namespace Tyra
