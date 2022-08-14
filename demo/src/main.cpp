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
#include "demo_game.hpp"

int main() {
  Tyra::Engine engine;
  Demo::DemoGame game(&engine);
  engine.run(&game);
  SleepThread();
  return 0;
}
