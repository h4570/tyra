/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Guido Diego Quispe Robles
*/

#pragma once

#include <tyra>

namespace Tyra {

class Tutorial11 : public Game {
 public:
  Tutorial11(Engine* engine);
  ~Tutorial11();

  void init();
  void loop();

 private:
  Engine* engine;
  Font font;
  Color white;
  Color skyBlue;
};

}  // namespace Tyra
