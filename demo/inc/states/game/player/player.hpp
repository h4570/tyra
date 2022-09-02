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

#include "states/game/shared/renderer/renderer_dynamic_pair.hpp"
#include "states/game/shared/renderer/renderer_static_pair.hpp"
#include "states/game/terrain/heightmap.hpp"
#include "./weapon.hpp"
#include "./camera.hpp"
#include "./player_shoot_action.hpp"
#include <tyra>

using Tyra::Engine;
using Tyra::Pad;
using Tyra::Renderer3DUtility;
using Tyra::Vec4;

namespace Demo {

class Player {
 public:
  Player(Engine* engine);
  ~Player();

  const Vec4& getPosition() const { return position; }
  CameraInfo3D getCameraInfo() { return camera.getCameraInfo(); }

  RendererStaticPair* pair;

  void update(const Heightmap& heightmap);

  PlayerShootAction getShootAction() const;

 private:
  void handlePlayerPosition(const Heightmap& heightmap,
                            const float& terrainHeight);

  float speed;
  Pad* pad;
  Vec4 position;
  Weapon weapon;
  Camera camera;
};

}  // namespace Demo
