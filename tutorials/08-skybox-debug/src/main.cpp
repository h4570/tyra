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
#include "tutorial_08.hpp"

/**
 * In this tutorial we will learn:
 * - How to render skybox and what is "ALLPASS" rendering method
 * - How to render debug lines and bboxes
 */

int main() {
  Tyra::Engine engine;
  Tyra::Tutorial08 game(&engine);
  engine.run(&game);
  return 0;
}
