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

namespace Tyra {

class Tutorial05 : public Game {
 public:
  Tutorial05(Engine* engine);
  ~Tutorial05();

  void init();
  void loop();

 private:
  void loadZombieMesh();
  void loadWarriorMesh();
  void warriorAnimationCallback(const AnimationSequenceCallback& callback);

  Engine* engine;

  Vec4 cameraPosition, cameraLookAt;

  std::unique_ptr<DynamicMesh> warriorMesh;
  std::unique_ptr<DynamicMesh> zombieMesh;
  DynamicPipeline dynpip;
  DynPipOptions renderOptions;
};

}  // namespace Tyra
