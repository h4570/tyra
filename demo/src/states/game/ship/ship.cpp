/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/ship/ship.hpp"
#include <loaders/3d/obj_loader/obj_loader.hpp>
#include <file/file_utils.hpp>

using Tyra::FileUtils;
using Tyra::ObjLoader;
using Tyra::ObjLoaderOptions;

namespace Demo {

Ship::Ship(TextureRepository* repo) {
  ObjLoader loader;

  ObjLoaderOptions objOptions;
  objOptions.flipUVs = true;
  objOptions.scale = 200.0F;

  auto* data =
      loader.load(FileUtils::fromCwd("game/models/ship/ship.obj"), objOptions);
  mesh = new StaticMesh(*data);
  delete data;

  for (std::size_t i = 0; i < mesh->materials.size(); i++) {
    mesh->materials[i]->ambient /= 2.0F;
    mesh->materials[i]->ambient.a = 128.0F;
  }

  mesh->setPosition(Vec4(-800.0F, 10.0F, -2500.0F));

  allocateOptions();

  pair = new RendererStaticPair{mesh, options};
}

Ship::~Ship() {
  delete mesh;
  delete options;
  delete pair;
}

void Ship::allocateOptions() { options = new StaPipOptions(); }

}  // namespace Demo
