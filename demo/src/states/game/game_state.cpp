/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/game_state.hpp"
#include "debug/debug.hpp"
#include "file/file_utils.hpp"

using Tyra::FileUtils;

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

  engine->audio.song.stop();
  engine->audio.song.load(FileUtils::fromCwd("game/ambiance.wav"));
  engine->audio.song.inLoop = true;
  engine->audio.song.setVolume(100);
  engine->audio.song.play();

  auto* repository = &engine->renderer.core.texture.repository;
  player = new Player(engine);
  enemy = new Enemy(repository);
  terrain = new Terrain(repository);
  skybox = new Skybox(repository);
  dbgObj = new DebugObject(repository);

  initialized = true;
}

GlobalStateType GameState::onFinish() {
  if (!initialized) return STATE_EXIT;

  delete player;
  delete enemy;
  delete terrain;
  delete skybox;
  delete dbgObj;
  initialized = false;

  return STATE_EXIT;
}

void GameState::update() {
  if (fpsChecker++ > 100) {
    TYRA_LOG("FPS: ", engine->info.getFps());
    fpsChecker = 0;
  }

  skybox->update(player->getPosition());
  auto cameraInfo = player->getCameraInfo();
  engine->renderer.beginFrame(cameraInfo);

  dbgObj->setPosition(*cameraInfo.looksAt);
  float terrainHeight = terrain->getHeightOffset(player->getPosition());
  player->update(terrainHeight);
  enemy->update(terrain->heightmap, player->getPosition());

  renderer.clear();
  {
    renderer.add(skybox->pair);
    renderer.add(player->pair);
    renderer.add(enemy->pair);
    // renderer.add(terrain->pair);
    renderer.add(dbgObj->pair);
  }
  renderer.render();

  engine->renderer.endFrame();
}

}  // namespace Demo
