/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022 - 2023, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# André Guilherme <andregui17@outlook.com>
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

struct EngineOptions {
  /**
   * True -> logs will be written to file.
   * False -> logs will be displayed in console
   */
  bool writeLogsToFile = false;

  bool loadUsbDriver = false;

  bool loadHddDriver = false;
};

class Engine {
 public:
  Engine();
  Engine(const EngineOptions& options);
  ~Engine();

  Renderer renderer;
  Pad pad;
  Audio audio;
  Info info;

  void run(Game* t_game);

 private:
  IrxLoader irx;

  Game* game;
  Banner banner;

  void realLoop();
  void initAll(const bool& loadUsbDriver, const bool& loadHddDriver);
};

}  // namespace Tyra
