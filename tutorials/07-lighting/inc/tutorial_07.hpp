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

class Tutorial07 : public Game {
 public:
  Tutorial07(Engine* engine);
  ~Tutorial07();

  void init();
  void loop();

 private:
  void loadMeshWithLightmap();
  void setDirectionalLightsOptions();
  void setLightmap(MeshBuilderData* data);
  void switchBetweenLightmapAndDirectional();

  int counter, switcher;

  Engine* engine;

  Vec4 cameraPosition, cameraLookAt;

  StaticPipeline stapip;
  StaPipOptions renderOptions;
  std::unique_ptr<StaticMesh> mesh;

  PipelineLightingOptions renderLightingOptions;
  Color directionalAmbientColor;
  std::array<Color, 3> directionalColors;
  std::array<Vec4, 3> directionalDirections;
};

}  // namespace Tyra
