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

#include <engine.hpp>
#include <game.hpp>
#include "renderer/3d/pipeline/minecraft/minecraft_pipeline.hpp"
#include "renderer/3d/pipeline/static/static_pipeline.hpp"
#include "renderer/3d/pipeline/dynamic/dynamic_pipeline.hpp"
#include "renderer/3d/mesh/static/static_mesh.hpp"

namespace Tyra {

class Tutorial01 : public Game {
 public:
  Tutorial01(Engine* engine);
  ~Tutorial01();

  void init();
  void loop();

 private:
  Engine* engine;

  StaticMesh* skybox;
  DynamicMesh* cube;
  StaticMesh* staticMesh;
  DynamicMesh* warrior;
  u8 warriorsCount;
  DynamicMesh** warriors;
  u32 picturesCount;
  Sprite** pictures;
  audsrv_adpcm_t* adpcmSample;
  Timer adpcmTimer;

  Vec4 cameraPosition, cameraLookAt;

  MinecraftPipeline mcPip;
  DynamicPipeline dynpip;
  StaticPipeline stapip;
  StaPipOptions* skyboxOptions;
  StaPipOptions* staOptions;
  DynPipOptions* dynOptions;
  Texture* warriorTex;
  Texture* blocksTex;
  Texture* blocksTex2;

  u32 blocksCount;
  std::vector<McpipBlock*> blocks;
  M4x4* translations;
  M4x4* rotations;
  M4x4* scales;
};

}  // namespace Tyra
