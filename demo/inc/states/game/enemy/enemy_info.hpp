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
  u8 adpcmChannel;
  audsrv_adpcm_t* adpcmSample;
  DynamicMesh* motherMesh;
  Texture* bodyTexture;
  Texture* clothTexture;
  Vec4 spawnPoint;
};

}  // namespace Demo
