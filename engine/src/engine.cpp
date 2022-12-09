/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# André Guilherme <andregui17@outlook.com>
*/

#include "engine.hpp"

namespace Tyra {

Engine::Engine() { initAll(false, false, false, false); }

Engine::Engine(const EngineOptions& options) {
  info.writeLogsToFile = options.writeLogsToFile;
  initAll(options.loadUsbDriver, options.loadHddDriver);
}

Engine::~Engine() {}

void Engine::run(Game* t_game) {
  game = t_game;
  game->init();
  while (true) {
    realLoop();
  }
}

void Engine::realLoop() {
  pad.update();
  game->loop();
  info.update();
}

void Engine::initAll(const bool& loadUsbDriver, const bool& loadHddDriver, const bool& loadCdFSDriver, const bool& loadMemcardDriver) {
  srand(time(nullptr));
  irx.loadAll(loadUsbDriver, loadHddDriver, loadCdFS_Driver, loadMemcardDriver, info.writeLogsToFile);
  renderer.init();
  banner.show(&renderer);
  audio.init();
  pad.init();
}

}  // namespace Tyra
