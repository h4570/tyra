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

#include "./renderer/renderer.hpp"
#include "./pad/pad.hpp"
#include "./audio/audio.hpp"
#include "./irx/irx_loader.hpp"
#include "./info/info.hpp"
#include "./info/banner.hpp"
#include "./game.hpp"

namespace Tyra {

class Engine {
 public:
  Engine();
  ~Engine();

  Renderer renderer;
  Pad pad;
  Audio audio;
  IrxLoader irx;
  Info info;

  void run(Game* t_game);

 private:
  Game* game;
  Banner banner;

  void realLoop();
};

}  // namespace Tyra
