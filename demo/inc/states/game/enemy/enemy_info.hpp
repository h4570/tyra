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
#include <audsrv.h>

using Tyra::DynamicMesh;
using Tyra::Texture;
using Tyra::Vec4;

namespace Demo {

struct EnemyInfo {
  unsigned char adpcmChannel;
  audsrv_adpcm_t* adpcmPunch;
  audsrv_adpcm_t* adpcmDeath;
  DynamicMesh* motherMesh;
  Texture* bodyTexture;
  Texture* clothTexture;
  Vec4 terrainLeftUp;
  Vec4 terrainRightDown;
};

}  // namespace Demo
