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
#include "tutorial_03.hpp"

/**
 * In this tutorial we will learn:
 * - What are the VU1 pipelines
 * - How to create a 3D camera
 * - What is the translation, rotation scale and model matrix
 * - How to render 3D minecraft cubes
 */

int main() {
  Tyra::Engine engine;
  Tyra::Tutorial03 game(&engine);
  engine.run(&game);
  return 0;
}
