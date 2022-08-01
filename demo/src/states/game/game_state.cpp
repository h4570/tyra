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
#include "debug/debug.hpp"

namespace Demo {

GameState::GameState(Engine* t_engine)
    : State(t_engine), renderer(&t_engine->renderer) {
  state = STATE_GAME;
  _wantFinish = false;
  initialized = false;
  fpsChecker = 0;
}

GameState::~GameState() {}

void GameState::onStart() {
  TYRA_LOG("Game. RAM: ", engine->info.getAvailableRAM(), "MB");

  engine->audio.stopSong();

  player = new Player(engine);
  terrain = new Terrain(&engine->renderer.core.texture.repository);
  dbgObj = new DebugObject(&engine->renderer.core.texture.repository);

  initialized = true;
}

GlobalStateType GameState::onFinish() {
  if (!initialized) return STATE_EXIT;

  delete player;
  delete terrain;
  delete dbgObj;
  initialized = false;

  return STATE_EXIT;
}

void GameState::update() {
  if (fpsChecker++ > 100) {
    TYRA_LOG("FPS: ", engine->info.getFps());
    fpsChecker = 0;
  }

  // TODO: Get next player position and calculate all stuf for NEXT position
  // (not current!)
  // TODO: Fix camera for heightmap
  // TODO: Change map
  // TODO: Implement collision

  auto cameraInfo = player->getCameraInfo();
  engine->renderer.beginFrame(cameraInfo);

  float terrainHeight = terrain->getHeightOffset(player->getPosition());
  player->update(terrainHeight);

  dbgObj->setPosition(*cameraInfo.looksAt);

  renderer.clear();
  {
    renderer.add(player->staticPairs);
    renderer.add(player->dynamicPairs);
    renderer.add(terrain->pair);
    renderer.add(dbgObj->pair);
  }
  renderer.render();

  engine->renderer.endFrame();
}

}  // namespace Demo
