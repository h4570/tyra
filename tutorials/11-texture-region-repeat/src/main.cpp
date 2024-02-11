/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2024, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include "engine.hpp"
#include "tutorial_11.hpp"

/**
 * In this tutorial we will learn:
 * - How to use texture in repeate mode of GS
 */

int main() {
  Tyra::Engine engine;
  Tyra::Tutorial11 game(&engine);
  engine.run(&game);
  return 0;
}
