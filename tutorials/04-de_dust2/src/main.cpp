/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "engine.hpp"
#include "tutorial_04.hpp"

/**
 * In this tutorial we will learn:
 * - How to get pad input and move the camera
 * - What is static pipeline and how to use it
 * - What is mesh (StaticMesh) and how to load it from .obj file
 */

int main() {
  Tyra::Engine engine;
  Tyra::Tutorial04 game(&engine);
  engine.run(&game);
  return 0;
}
