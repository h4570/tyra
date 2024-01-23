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
#include "tutorial_11.hpp"

/**
 * In this tutorial we will learn how to:
 * - Load and unload fonts
 */

int main() {
  Tyra::Engine engine;
  Tyra::Tutorial11 game(&engine);
  engine.run(&game);
  return 0;
}
