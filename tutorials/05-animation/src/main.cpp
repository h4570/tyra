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
#include "tutorial_05.hpp"

/**
 * In this tutorial we will learn:
 * - What is a DynamicMesh and DynamicPipeline
 * - How to load animated model from .obj and .md2 format
 * - How to arrange animation keyframes
 */

int main() {
  Tyra::Engine engine;
  Tyra::Tutorial05 game(&engine);
  engine.run(&game);
  return 0;
}
