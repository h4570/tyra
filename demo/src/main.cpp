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
#include "game_settings.hpp"

int main() {
  Tyra::EngineOptions options;

  if (Demo::IS_REAL_PS2_VIA_USB) {
    options.writeLogsToFile = true;
    options.loadUsbDriver = true;
  }
  
  if (Demo::IS_REAL_PS2_VIA_HDD) {
    options.writeLogsToFile = true;
    options.loadHddDriver = true;
  }

  Tyra::Engine engine(options);

  Demo::DemoGame game(&engine);
  engine.run(&game);
  SleepThread();
  return 0;
}
