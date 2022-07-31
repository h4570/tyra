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
#include <renderer/renderer.hpp>

using Tyra::Renderer;
using Tyra::StaPipOptions;
using Tyra::StaticMesh;
using Tyra::TextureRepository;
using Tyra::Vec4;

namespace Demo {

class Weapon {
 public:
  Weapon(TextureRepository* repo);
  ~Weapon();

  StaticMesh* mesh;
  StaPipOptions* options;

  void update();
  // void debugPosition(Pad* pad);

 private:
  void allocateOptions();
};

}  // namespace Demo
