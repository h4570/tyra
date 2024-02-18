/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Guido Diego Quispe Robles
*/

#include "engine.hpp"
#include "tutorial_12.hpp"

/**
 * In this tutorial we will learn how to:
 * - Load and unload fonts
 * - Load an individual character
 * - How works the limit textures
 */

int main() {
  Tyra::Engine engine;
  Tyra::Tutorial12 game(&engine);
  engine.run(&game);
  return 0;
}
