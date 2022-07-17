/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include "engine.hpp"
#include "wellinator.hpp"

int main() {
  Tyra::Engine engine;
  Tyra::Wellinator game(&engine);
  engine.run(&game);
  SleepThread();
  return 0;
}
