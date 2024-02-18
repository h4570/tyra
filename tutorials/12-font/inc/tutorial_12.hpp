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

class Tutorial12 : public Game {
 public:
  explicit Tutorial12(Engine* engine);
  ~Tutorial12();

  void init();
  void loop();

 private:
  Engine* engine;
  FontData myFont;
  FontData myEmojiFont;
  Color white;
  Color skyBlue;
};

}  // namespace Tyra
