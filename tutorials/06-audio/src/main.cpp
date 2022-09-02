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
#include "tutorial_06.hpp"

/**
 * In this tutorial we will learn:
 * - How to play background music
 * - How to play sound effects (ADPCM samples)
 */

int main() {
  Tyra::Engine engine;
  Tyra::Tutorial06 game(&engine);
  engine.run(&game);
  return 0;
}
