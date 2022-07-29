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

#include <engine.hpp>
#include <game.hpp>

namespace Tyra {

class Demo : public Game {
 public:
  Demo(Engine* engine);
  ~Demo();

  void init();
  void loop();

 private:
  Engine* engine;
};

}  // namespace Tyra
