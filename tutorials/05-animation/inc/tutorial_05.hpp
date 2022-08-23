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

namespace Tyra {

class Tutorial05 : public Game {
 public:
  Tutorial05(Engine* engine);
  ~Tutorial05();

  void init();
  void loop();

 private:
  Engine* engine;
};

}  // namespace Tyra
