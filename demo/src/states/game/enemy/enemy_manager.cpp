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

#include "states/game/enemy/enemy_manager.hpp"
#include <functional>
#include "game_settings.hpp"

using Tyra::FileUtils;
using Tyra::Math;
using Tyra::ObjLoader;
using Tyra::ObjLoaderOptions;

namespace Demo {

EnemyManager::EnemyManager(Engine* engine, const Heightmap& heightmap) {
  textureRepo = &engine->renderer.core.texture.repository;

  ObjLoader loader;
  ObjLoaderOptions objOptions;
  objOptions.animation.count = 6;
  objOptions.flipUVs = true;
  objOptions.scale = 250.0F;

  auto data = loader.load(FileUtils::fromCwd("game/models/zombie/zombie.obj"),
                          objOptions);
  data->loadNormals = false;
  motherMesh = new DynamicMesh(data.get());

  bodyTexture = textureRepo->add(
      FileUtils::fromCwd("game/models/zombie/BodyMaterial.png"));

  clothTexture = textureRepo->add(
      FileUtils::fromCwd("game/models/zombie/ClothMaterial.png"));

  auto* punch = engine->audio.adpcm.load(
      FileUtils::fromCwd("game/models/zombie/punch.adpcm"));

  auto* death = engine->audio.adpcm.load(
      FileUtils::fromCwd("game/models/zombie/death.adpcm"));

  const int enemyCount = IS_REAL_PS2_VIA_USB | IS_REAL_PS2_VIA_HDD ? 8 : 12;
  for (int i = 0; i < enemyCount; i++) {
    EnemyInfo info;
    info.adpcmChannel = 9 + i;
    info.adpcmPunch = punch;
    info.adpcmDeath = death;
    info.motherMesh = motherMesh;
    info.clothTexture = clothTexture;
    info.bodyTexture = bodyTexture;
    info.terrainLeftUp = heightmap.leftUp;
    info.terrainRightDown = heightmap.rightDown;
    enemies.push_back(new Enemy(engine, info));
  }
}

EnemyManager::~EnemyManager() {
  for (auto* enemy : enemies) {
    delete enemy;
  }

  delete motherMesh;

  textureRepo->free(bodyTexture->id);
  textureRepo->free(clothTexture->id);
}

std::vector<RendererDynamicPair*> EnemyManager::getPairs() const {
  std::vector<RendererDynamicPair*> result;

  for (auto* enemy : enemies) {
    result.push_back(enemy->pair);
  }

  return result;
}

void EnemyManager::update(const Heightmap& heightmap,
                          const Vec4& playerPosition,
                          const PlayerShootAction& shootAction) {
  for (auto* enemy : enemies) {
    enemy->update(heightmap, playerPosition, shootAction);
  }
}

}  // namespace Demo
