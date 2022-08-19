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

#include <tyra>
#include "states/game/renderer/renderer_dynamic_pair.hpp"
#include "states/game/terrain/heightmap.hpp"
#include "states/game/player/player_shoot_action.hpp"
#include "./enemy_info.hpp"

using Tyra::AnimationSequenceCallback;
using Tyra::Audio;
using Tyra::DynamicMesh;
using Tyra::DynPipOptions;
using Tyra::Engine;
using Tyra::Renderer;
using Tyra::Renderer3DUtility;
using Tyra::TextureRepository;
using Tyra::Vec4;

namespace Demo {

class Enemy {
 public:
  Enemy(Engine* engine, const EnemyInfo& info);
  ~Enemy();

  DynamicMesh* mesh;
  DynPipOptions* options;
  RendererDynamicPair* pair;

  void update(const Heightmap& heightmap, const Vec4& playerPosition,
              const PlayerShootAction& shootAction);

 private:
  std::vector<u32> walkSequence;
  std::vector<u32> fightSequence;

  void allocateOptions();
  void walk(const Heightmap& heightmap, const Vec4& positionDiff);
  void animationCallback(const AnimationSequenceCallback& callback);
  void fight();
  void handlePlayerShoot(const PlayerShootAction& shootAction);
  void setMeshToSpawn();

  bool isWalking = true;
  bool isFighting = false;
  Vec4 terrainLeftUp;
  Vec4 terrainRightDown;

  Audio* audio;
  EnemyInfo info;
};

}  // namespace Demo
