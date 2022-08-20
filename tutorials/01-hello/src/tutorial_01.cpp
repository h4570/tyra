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

namespace Tyra {

/** Lets save pointer to engine class */
Tutorial01::Tutorial01(Engine* t_engine) : engine(t_engine) {}

Tutorial01::~Tutorial01() {}

void Tutorial01::init() {
  /**
   * There are 5 debug functions in Tyra engine.
   *
   * TYRA_LOG() -> Prints log message
   *
   * TYRA_WARN() -> Prints warning message
   *
   * TYRA_ERROR() -> Prints error message
   *
   * TYRA_ASSERT() -> Checks if condition is true, otherwise prints error
   * message
   *
   * TYRA_BREAKPOINT() -> Freeze program in current place
   *
   * Every function has infinite number of various-typed arguments.
   */

  short positiveNumber = -1;  // Change it to -1 and see what happen!

  TYRA_LOG("Hello world!");

  TYRA_WARN("Something weird happened! ",
            "Number should be positive, but is: ", -1);

  TYRA_ERROR("Something really bad happened!");

  /** First argument is our check argument */
  TYRA_ASSERT(positiveNumber > 0, "Number should be positive!",
              "Please check the code!", "Current value:", (int)positiveNumber);
}

void Tutorial01::loop() {
  /** Uncomment following line, to check the game loop! */

  // TYRA_LOG("Loop!");
}

}  // namespace Tyra
