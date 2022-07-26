/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
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

class H4570 : public Game {
 public:
  H4570(Engine* engine);
  ~H4570();

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
  Sprite* picture;
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

  u32 blocksCount;
  McpipBlock* blocks;
  M4x4* translations;
  M4x4* rotations;
  M4x4* scales;
};

}  // namespace Tyra
