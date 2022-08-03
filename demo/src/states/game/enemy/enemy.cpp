/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/enemy/enemy.hpp"
#include <loaders/3d/tyrobj/tyrobj_loader.hpp>
#include <file/file_utils.hpp>

using Tyra::FileUtils;
using Tyra::TyrobjLoader;

namespace Demo {

Enemy::Enemy(TextureRepository* repo) {
  TyrobjLoader loader;

  auto* bodyData = loader.load(
      FileUtils::fromCwd("game/models/soldier/soldier.obj"), 4, 30.0F, true);
  bodyData->normalsEnabled = false;
  bodyMesh = new DynamicMesh(*bodyData);

  TYRA_LOG("First vertex: ", bodyData->frames[0]->vertices[0].getPrint());
  TYRA_LOG("Last vertex: ",
           bodyData->frames[0]
               ->vertices[bodyData->frames[0]->verticesCount - 1]
               .getPrint());

  TYRA_LOG("First index: ", bodyData->materials[0]->vertexFaces[0]);
  TYRA_LOG(
      "Last index: ",
      bodyData->materials[0]->vertexFaces[bodyData->materials[0]->count - 1]);

  delete bodyData;

  bodyMesh->getMaterial(0)->color.r = 16.0F;
  bodyMesh->getMaterial(0)->color.g = 16.0F;
  bodyMesh->getMaterial(0)->color.b = 16.0F;

  bodyMesh->playAnimation(0, bodyMesh->getFramesCount() - 1);
  bodyMesh->setAnimSpeed(0.15F);

  repo->addByMesh(bodyMesh, FileUtils::fromCwd("game/models/soldier/"), "png");

  // auto* headData = loader.load(
  //     FileUtils::fromCwd("game/models/soldier/head.md2"), scale, true);
  // headData->normalsEnabled = false;
  // headMesh = new DynamicMesh(*headData);
  // delete headData;

  // headMesh->getMaterial(0)->color.r = 16.0F;
  // headMesh->getMaterial(0)->color.g = 16.0F;
  // headMesh->getMaterial(0)->color.b = 16.0F;

  // repo->addByMesh(headMesh, FileUtils::fromCwd("game/models/soldier/"),
  // "png");

  allocateOptions();

  pairs.push_back(new RendererDynamicPair{bodyMesh, options});
  // pairs.push_back(new RendererDynamicPair{gunMesh, options});
}

Enemy::~Enemy() {
  delete bodyMesh;
  // delete gunMesh;
  delete options;
  for (auto* pair : pairs) {
    delete pair;
  }
}

void Enemy::update(const Vec4& playerPosition) { bodyMesh->animate(); }

void Enemy::allocateOptions() { options = new DynPipOptions(); }

}  // namespace Demo
