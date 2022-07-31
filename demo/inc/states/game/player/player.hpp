/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "states/game/renderer/renderer_dynamic_pair.hpp"
#include "states/game/renderer/renderer_static_pair.hpp"
#include "./weapon.hpp"
#include "./camera.hpp"
#include <engine.hpp>

using Tyra::Engine;
using Tyra::Pad;
using Tyra::Vec4;

namespace Demo {

class Player {
 public:
  Player(Engine* engine);
  ~Player();

  CameraInfo3D getCameraInfo() { return camera.getCameraInfo(); }

  std::vector<RendererStaticPair*> staticPairs;
  std::vector<RendererDynamicPair*> dynamicPairs;

  void update();

 private:
  void handlePlayerPosition();

float speed;
  Pad* pad;
  Vec4 position;
  Weapon weapon;
  Camera camera;
};

}  // namespace Demo
