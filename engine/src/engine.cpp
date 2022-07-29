/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "engine.hpp"

namespace Tyra {

Engine::Engine() {
  irx.loadDefaultDrivers();
  audio.init();
  pad.init();
  renderer.init();
  srand(time(nullptr));
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

}  // namespace Tyra
