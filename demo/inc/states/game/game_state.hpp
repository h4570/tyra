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

#include "state/state.hpp"
#include "state/global_state_type.hpp"
#include "./player/player.hpp"
#include "./shared/renderer/game_renderer.hpp"
#include "./terrain/terrain.hpp"
#include "./skybox/skybox.hpp"
#include "./ship/ship.hpp"
#include "./hud/hud.hpp"
#include "./enemy/enemy_manager.hpp"

using std::unique_ptr;

namespace Demo {

class GameState : public State<GlobalStateType> {
 public:
  GameState(Engine* t_engine);
  ~GameState();

  const GlobalStateType& getState() const { return state; }

  const bool& wantFinish() const { return _wantFinish; };

  void onStart();

  void update();

  /** @return Next game state */
  GlobalStateType onFinish();

 private:
  GlobalStateType state;
  bool _wantFinish;
  bool initialized;
  unsigned char fpsChecker;

  GameRenderer renderer;
  unique_ptr<Player> player;
  unique_ptr<EnemyManager> enemyManager;
  unique_ptr<Terrain> terrain;
  unique_ptr<Skybox> skybox;
  unique_ptr<Ship> ship;
  unique_ptr<Hud> hud;
};

}  // namespace Demo
