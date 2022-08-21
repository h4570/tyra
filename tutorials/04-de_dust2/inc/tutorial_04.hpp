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

#include <tyra>
#include <memory.h>

namespace Tyra {

class Tutorial04 : public Game {
 public:
  Tutorial04(Engine* engine);
  ~Tutorial04();

  void init();
  void loop();

 private:
  void loadMesh();

  Engine* engine;

  std::unique_ptr<StaticMesh> mesh;
  StaPipOptions renderOptions;
};

}  // namespace Tyra
