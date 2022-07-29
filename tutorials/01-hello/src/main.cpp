/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "engine.hpp"
#include "tutorial_01.hpp"

int main() {
  Tyra::Engine engine;
  Tyra::Tutorial01 game(&engine);
  engine.run(&game);
  SleepThread();
  return 0;
}
