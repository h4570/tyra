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

/** We can include all of Tyra classes via this single line: */
#include <tyra>

namespace Tyra {

/** Tutorial01 is a name of our game class */
class Tutorial01 : public Game {
 public:
  Tutorial01(Engine* engine);
  ~Tutorial01();

  /**
   * This function will be called once,
   * immediatly after initializing Tyra features
   */
  void init();

  /**
   * This function will be called every frame
   */
  void loop();

 private:
  Engine* engine;
};

}  // namespace Tyra
