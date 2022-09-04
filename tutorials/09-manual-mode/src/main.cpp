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
#include "tutorial_09.hpp"

/**
 * In this tutorial we will learn:
 * - What is StaticPipeline/DynamicPipeline core class
 * - How to render data manually (without Mesh class)
 * - What are the "3D bags"
 */

int main() {
  Tyra::Engine engine;
  Tyra::Tutorial09 game(&engine);
  engine.run(&game);
  return 0;
}
