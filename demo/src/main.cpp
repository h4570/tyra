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
#include "game_settings.hpp"

int main() {
  Tyra::EngineOptions options;

  if (Demo::IS_REAL_PS2_VIA_USB) {
    options.loggingMode = LOGGING_FILE;
    options.loadUsbDriver = true;
  }

  Tyra::Engine engine(options);

  Demo::DemoGame game(&engine);
  engine.run(&game);
  SleepThread();
  return 0;
}
