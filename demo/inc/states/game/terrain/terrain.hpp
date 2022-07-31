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

#include <renderer/3d/mesh/static/static_mesh.hpp>
#include <renderer/3d/pipeline/static/stapip_options.hpp>
#include "states/game/renderer/renderer_static_pair.hpp"
#include <renderer/renderer.hpp>

using Tyra::StaPipOptions;
using Tyra::StaticMesh;
using Tyra::TextureRepository;

namespace Demo {

class Terrain {
 public:
  Terrain(TextureRepository* repo);
  ~Terrain();

  StaticMesh* mesh;
  StaPipOptions* options;
  RendererStaticPair* pair;

  void update();

 private:
  void allocateOptions();
};

}  // namespace Demo
