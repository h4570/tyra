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

#include <engine.hpp>
#include <game.hpp>
#include "state/state_manager.hpp"

using Tyra::Engine;
using Tyra::Game;

namespace Demo {

class DemoGame : public Game {
 public:
  DemoGame(Engine* engine);
  ~DemoGame();

  void init();
  void loop();

 private:
  Engine* engine;
  StateManager<GlobalStateType> stateManager;
};

}  // namespace Demo
