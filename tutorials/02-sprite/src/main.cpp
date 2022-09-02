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
#include "tutorial_02.hpp"

/**
 * In this tutorial we will learn how to:
 * - Load png file and create texture from it
 * - Create sprite class
 * - Clear screen and render sprite
 */

int main() {
  Tyra::Engine engine;
  Tyra::Tutorial02 game(&engine);
  engine.run(&game);
  return 0;
}
