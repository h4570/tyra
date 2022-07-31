/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/game_state.hpp"
#include "file/file_utils.hpp"
#include "thread/threading.hpp"
#include "debug/debug.hpp"

using Tyra::FileUtils;
using Tyra::Threading;

namespace Demo {

GameState::GameState(Engine* t_engine) : State(t_engine) {
  state = STATE_GAME;
  _wantFinish = false;
  initialized = false;
}

GameState::~GameState() {}

void GameState::onStart() {
  TYRA_LOG("Game. RAM: ", engine->info.getAvailableRAM(), "MB");

  engine->audio.stopSong();
  initialized = true;
}

GlobalStateType GameState::onFinish() {
  if (!initialized) return STATE_EXIT;

  initialized = false;

  return STATE_EXIT;
}

void GameState::update() {
  engine->renderer.beginFrame();
  Threading::switchThread();
  engine->renderer.endFrame();
}

}  // namespace Demo
