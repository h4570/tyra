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
#include "renderer/3d/pipeline/std/std_pipeline.hpp"

namespace Tyra {

class H4570 : public Game {
 public:
  H4570(Engine* engine);
  ~H4570();

  void init();
  void loop();

 private:
  Engine* engine;

  Mesh* warrior;
  Mesh* warrior2;
  Mesh* warrior3;
  Mesh* warrior4;
  Sprite* picture;
  audsrv_adpcm_t* adpcmSample;
  Timer adpcmTimer;

  Vec4 cameraPosition, cameraLookAt;

  MinecraftPipeline mcPip;
  StdPipeline stdPip;
  StdpipOptions* renderOptions;
  Texture* blocksTex;

  u32 blocksCount;
  McpipBlock* blocks;
  M4x4* translations;
  M4x4* rotations;
  M4x4* scales;
};

}  // namespace Tyra
