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

#include <renderer/3d/mesh/dynamic/dynamic_mesh.hpp>
#include <renderer/3d/pipeline/dynamic/dynpip_options.hpp>
#include <renderer/3d/mesh/dynamic/animation_sequence_callback.hpp>
#include "states/game/renderer/renderer_dynamic_pair.hpp"
#include "states/game/terrain/heightmap.hpp"
#include <renderer/renderer.hpp>
#include <engine.hpp>
#include "./enemy_info.hpp"

using Tyra::AnimationSequenceCallback;
using Tyra::Audio;
using Tyra::DynamicMesh;
using Tyra::DynPipOptions;
using Tyra::Engine;
using Tyra::Renderer;
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

  void update(const Heightmap& heightmap, const Vec4& playerPosition);

 private:
  std::vector<u32> walkSequence;
  std::vector<u32> fightSequence;

  void allocateOptions();
  void walk(const Heightmap& heightmap, const Vec4& positionDiff);
  void animationCallback(const AnimationSequenceCallback& callback);
  void fight();

  bool isWalking = true;
  bool isFighting = false;

  Audio* audio;
  EnemyInfo info;
};

}  // namespace Demo
