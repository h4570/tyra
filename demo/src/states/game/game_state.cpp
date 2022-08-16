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
  terrain = new Terrain(repository);
  enemyManager = new EnemyManager(engine, terrain->heightmap);
  ship = new Ship(repository);
  skybox = new Skybox(repository);

  initialized = true;
}

GlobalStateType GameState::onFinish() {
  if (!initialized) return STATE_EXIT;

  delete player;
  delete enemyManager;
  delete terrain;
  delete ship;
  delete skybox;
  initialized = false;

  return STATE_EXIT;
}

void GameState::update() {
  // Begin frame
  player->update(terrain->heightmap);
  auto cameraInfo = player->getCameraInfo();
  engine->renderer.beginFrame(cameraInfo);

  // Clear screen and render skybox
  renderer.clear();
  skybox->update(player->getPosition());
  renderer.renderSkybox(*skybox->pair);  // First, because of ALLPASS

  // Check fps
  if (fpsChecker++ > 50) {
    TYRA_LOG("FPS: ", engine->info.getFps(),
             " RAM: ", engine->info.getAvailableRAM());
    fpsChecker = 0;
  }

  // Game logic
  auto shootAction = player->getShootAction();
  enemyManager->update(terrain->heightmap, player->getPosition(), shootAction);

  // Render other stuff
  renderer.add(ship->pair);
  renderer.add(player->pair);
  renderer.add(enemyManager->getPairs());
  renderer.add(terrain->pair);
  renderer.render();

  // Debug
  auto& utility = engine->renderer.renderer3D.utility;
  utility.drawBox(*player->getCameraInfo().looksAt, 0.3F);

  auto* firstEnemy = enemyManager->getPairs().front()->mesh;
  utility.drawBBox(firstEnemy->getCurrentBoundingBox().getTransformed(
      firstEnemy->getModelMatrix()));

  // End frame
  engine->renderer.endFrame();
}

}  // namespace Demo
