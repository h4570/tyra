/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022 - 2023, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/game_state.hpp"
#include "game_settings.hpp"

using std::make_unique;
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
  engine->audio.song.inLoop = true;
  engine->audio.song.load(FileUtils::fromCwd("game/game-audio.wav"));
  engine->audio.song.setVolume(0);
  engine->audio.song.play();

  auto* repository = &engine->renderer.core.texture.repository;
  player = make_unique<Player>(engine);
  terrain = make_unique<Terrain>(repository);
  enemyManager = make_unique<EnemyManager>(engine, terrain->heightmap);
  ship = make_unique<Ship>(repository);
  skybox = make_unique<Skybox>(repository);
  hud = make_unique<Hud>(repository);

  engine->audio.song.setVolume(85);

  if (IS_REAL_PS2_VIA_USB || IS_REAL_PS2_VIA_HDD) engine->renderer.setFrameLimit(false);

  initialized = true;
}

GlobalStateType GameState::onFinish() {
  if (!initialized) return STATE_EXIT;

  initialized = false;

  return STATE_EXIT;
}

void GameState::update() {
  // Check fps
  if (fpsChecker++ > 50) {
    TYRA_LOG("FPS: ", engine->info.getFps(),
             " RAM: ", engine->info.getAvailableRAM());
    fpsChecker = 0;
  }

  // Game logic
  player->update(terrain->heightmap);
  skybox->update(player->getPosition());
  auto shootAction = player->getShootAction();
  enemyManager->update(terrain->heightmap, player->getPosition(), shootAction);
  auto* firstEnemyMesh = enemyManager->getPairs().front()->mesh;

  // Render
  engine->renderer.beginFrame(player->getCameraInfo());
  {
    renderer.clear();
    {
      renderer.add(skybox->pair);  // First, because of ALLPASS
      renderer.add(ship->pair);
      renderer.add(player->pair);
      renderer.add(enemyManager->getPairs());
      renderer.add(terrain->pair);
      renderer.add(firstEnemyMesh->getCurrentBoundingBox().getTransformed(
          firstEnemyMesh->getModelMatrix()));
      renderer.add(hud->crosshairSprite.get());
      renderer.add(hud->hpSprite.get());
      renderer.add(hud->soldierSprite.get());
    }
    renderer.render();
  }
  engine->renderer.endFrame();
}

}  // namespace Demo
