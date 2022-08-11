/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "demo_game.hpp"
#include "states/intro/intro_state.hpp"
#include "states/game/game_state.hpp"

namespace Demo {

DemoGame::DemoGame(Engine* t_engine)
    : engine(t_engine), stateManager(STATE_GAME, STATE_EXIT) {}
DemoGame::~DemoGame() {}

void DemoGame::init() {
  stateManager.add(new IntroState(engine));
  stateManager.add(new GameState(engine));
}
void DemoGame::loop() {
  stateManager.update();

  if (stateManager.finished()) {
    exit(0);
  }
}

}  // namespace Demo
