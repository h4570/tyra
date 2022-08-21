/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tyra>
#include "tutorial_04.hpp"

namespace Tyra {

Tutorial04::Tutorial04(Engine* t_engine) : engine(t_engine) {}

Tutorial04::~Tutorial04() {
  engine->renderer.getTextureRepository().freeByMesh(mesh.get());
}

void Tutorial04::init() { loadMesh(); }

void Tutorial04::loop() {}

void Tutorial04::loadMesh() {
  ObjLoader loader;
  ObjLoaderOptions options;
  options.flipUVs = true;
  options.scale = 200.0F;
  auto data = loader.load(FileUtils::fromCwd("de_dust2/de_dust2.obj"), options);

  mesh = std::make_unique<StaticMesh>(data.get());

  engine->renderer.getTextureRepository().addByMesh(
      mesh.get(), FileUtils::fromCwd("de_dust2/"), "png");
}

}  // namespace Tyra
