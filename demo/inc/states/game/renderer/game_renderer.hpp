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

#include "renderer/renderer.hpp"
#include "renderer/3d/pipeline/dynamic/dynamic_pipeline.hpp"
#include "renderer/3d/pipeline/static/static_pipeline.hpp"
#include "./renderer_dynamic_pair.hpp"
#include "./renderer_static_pair.hpp"

using Tyra::DynamicPipeline;
using Tyra::Renderer;
using Tyra::StaticPipeline;

namespace Demo {

class GameRenderer {
 public:
  GameRenderer(Renderer* renderer);
  ~GameRenderer();

  void add(std::vector<RendererStaticPair*> staticPairs);
  void add(std::vector<RendererDynamicPair*> dynamicPairs);
  void add(RendererStaticPair* staticPair);
  void add(RendererDynamicPair* dynamicPair);

  void clear();

  void render();

 private:
  Renderer* renderer;

  StaticPipeline stpip;
  DynamicPipeline dypip;

  std::vector<RendererStaticPair*> staticPairs;
  std::vector<RendererDynamicPair*> dynamicPairs;
};

}  // namespace Demo
