/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/enemy/enemy_manager.hpp"
#include <functional>

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

  auto* data = loader.load(FileUtils::fromCwd("game/models/zombie/zombie.obj"),
                           objOptions);
  data->normalsEnabled = false;
  motherMesh = new DynamicMesh(*data);

  delete data;

  bodyTexture = textureRepo->add(
      FileUtils::fromCwd("game/models/zombie/BodyMaterial.png"));

  clothTexture = textureRepo->add(
      FileUtils::fromCwd("game/models/zombie/ClothMaterial.png"));

  auto* sample = engine->audio.adpcm.load(
      FileUtils::fromCwd("game/models/zombie/punch.adpcm"));

  const int enemyCount = 5;
  for (int i = 0; i < enemyCount; i++) {
    EnemyInfo info;
    info.adpcmChannel = 9 + i;
    info.adpcmSample = sample;
    info.motherMesh = motherMesh;
    info.clothTexture = clothTexture;
    info.bodyTexture = bodyTexture;
    info.spawnPoint =
        Vec4(Math::randomf(heightmap.leftUp.x, heightmap.rightDown.x), 0.0F,
             Math::randomf(heightmap.leftUp.z, heightmap.rightDown.z), 1.0F);
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
