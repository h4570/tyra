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

class Tutorial08 : public Game {
 public:
  Tutorial08(Engine* engine);
  ~Tutorial08();

  void init();
  void loop();

 private:
  void loadCupMesh();
  void loadSkyboxMesh();

  Vec4 cameraPosition;
  Vec4 cameraLookAt;

  Engine* engine;
  unsigned char fpsCounter;

  StaticPipeline stapip;
  StaPipOptions skyboxRenderOptions;
  StaPipOptions cupRenderOptions;
  std::unique_ptr<StaticMesh> skyboxMesh;
  std::unique_ptr<StaticMesh> cupMesh;
};

}  // namespace Tyra
