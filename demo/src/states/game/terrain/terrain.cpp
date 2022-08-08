/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/terrain/terrain.hpp"
#include <loaders/3d/obj_loader/obj_loader.hpp>
#include <file/file_utils.hpp>

using Tyra::FileUtils;
using Tyra::ObjLoader;

namespace Demo {
Terrain::Terrain(TextureRepository* repo)
    : heightmap(-5.0F,                               // Min height
                100.0F,                              // Max height
                Vec4(-196.6F, 0.0F, -412.0F, 1.0F),  // Left up
                Vec4(286.0F, 0.0F, 443.3F, 1.0F)     // Right down
      ) {
  ObjLoader loader;
  auto* data = loader.load(
      FileUtils::fromCwd("game/models/terrain/terrain.obj"), 1, 25.0F, true);
  data->normalsEnabled = false;
  mesh = new StaticMesh(*data);
  mesh->getMaterial(0)->color.r = 32.0F;
  mesh->getMaterial(0)->color.g = 32.0F;
  mesh->getMaterial(0)->color.b = 32.0F;
  delete data;

  repo->addByMesh(mesh, FileUtils::fromCwd("game/models/terrain/"), "png");

  allocateOptions();

  pair = new RendererStaticPair{mesh, options};
}

float Terrain::getHeightOffset(const Vec4& playerPosition) {
  return heightmap.getHeightOffset(playerPosition);
}

Terrain::~Terrain() {
  delete mesh;
  delete options;
  delete pair;
}

void Terrain::allocateOptions() {
  options = new StaPipOptions();
  options->shadingType = Tyra::TyraShadingGouraud;
  options->blendingEnabled = true;
  options->antiAliasingEnabled = false;
}

}  // namespace Demo
