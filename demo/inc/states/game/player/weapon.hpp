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

using Tyra::Audio;
using Tyra::Engine;
using Tyra::Pad;
using Tyra::Renderer;
using Tyra::StaPipOptions;
using Tyra::StaticMesh;
using Tyra::TextureRepository;
using Tyra::Timer;
using Tyra::Vec4;

namespace Demo {

class Weapon {
 public:
  Weapon(Engine* engine);
  ~Weapon();

  StaticMesh* mesh;
  StaPipOptions* options;
  bool isShooting = false;

  void update();

 private:
  bool isShootAnimation1, isShootAnimation2;
  Pad* pad;
  Audio* audio;
  Timer shootTimer;
  Vec4 initialPosition;
  u8 adpcmCurrentChannel;
  u8 adpcmChannelsCount;

  audsrv_adpcm_t* shootAdpcm;

  void shoot();
  void allocateOptions();
  u8 getShootChannel();
};

}  // namespace Demo
