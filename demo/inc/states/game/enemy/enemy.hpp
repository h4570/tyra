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

#include <renderer/3d/mesh/dynamic/dynamic_mesh.hpp>
#include <renderer/3d/pipeline/dynamic/dynpip_options.hpp>
#include "states/game/renderer/renderer_dynamic_pair.hpp"
#include "states/game/terrain/heightmap.hpp"
#include <renderer/renderer.hpp>

using Tyra::DynamicMesh;
using Tyra::DynPipOptions;
using Tyra::Renderer;
using Tyra::TextureRepository;
using Tyra::Vec4;

namespace Demo {

class Enemy {
 public:
  Enemy(TextureRepository* repo);
  ~Enemy();

  DynamicMesh* mesh;
  DynPipOptions* options;
  RendererDynamicPair* pair;

  void update(const Heightmap& heightmap, const Vec4& playerPosition);

 private:
  Vec4 direction;
  void allocateOptions();
};

}  // namespace Demo
