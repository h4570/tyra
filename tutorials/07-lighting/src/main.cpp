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
#include "tutorial_07.hpp"

/**
 * In this tutorial we will learn:
 * - How to add directional lights to mesh
 * - How to add lightmap to mesh
 */

int main() {
  Tyra::Engine engine;
  Tyra::Tutorial07 game(&engine);
  engine.run(&game);
  return 0;
}
