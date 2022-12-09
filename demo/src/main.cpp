/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# André Guilherme <andregui17@outlook.com>
*/

#include <tyra>
#include "demo_game.hpp"

int main() {
  Tyra::EngineOptions options;

  options.loadUsbDriver = true;
  options.writeLogsToFile = true;
  options.loadHddDriver = true;
  options.loadUsbDriver = true;
  options.loadMemcardDriver = true;
  options.loadCdFSDriver = true;

  Tyra::Engine engine(options);

  Game game(&engine);
  engine.run(&game);
  SleepThread();
  return 0;
}
