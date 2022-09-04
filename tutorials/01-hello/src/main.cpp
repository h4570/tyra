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
#include "tutorial_01.hpp"

/**
 * Hello, thank you for choosing Tyra engine!
 *
 * In this tutorial we will learn how to:
 * - Initialize engine and game class
 * - Use debug functions
 * - Print "Hello world" to the screen
 */

int main() {
  /**
   * This is a class which contains
   * all features of Tyra.
   */
  Tyra::Engine engine;

  /**
   * This is a class with our game code.
   */
  Tyra::Tutorial01 game(&engine);

  /**
   * We need to insert our game class into engine.
   */
  engine.run(&game);

  /** Continuation in file tutorial_01.hpp */

  return 0;
}
