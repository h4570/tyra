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
#include <memory.h>
#include "./camera.hpp"

namespace Tyra {

class Tutorial04 : public Game {
 public:
  Tutorial04(Engine* engine);
  ~Tutorial04();

  void init();
  void loop();

 private:
  void loadMesh();

  Engine* engine;

  Camera camera;

  /** 3D pipeline used for rendering static meshes */
  StaticPipeline stapip;

  /** Options for static pipeline */
  StaPipOptions renderOptions;

  /** Mesh with de_dust2 */
  std::unique_ptr<StaticMesh> mesh;
};

}  // namespace Tyra
