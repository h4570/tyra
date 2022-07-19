/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020-2022, tyra - https://github.com/h4570/tyrav2
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# Sandro Wellinator <wellcoj@gmail.com>
*/

#include "info/info.hpp"

namespace Tyra {

Info::Info() {
  fps = 0;
  fpsDelayer = 0;
}

Info::~Info() {}

void Info::update() {
  if (fpsDelayer++ >= 4) {
    fps = calcFps();
    fpsDelayer = 0;
  }
  timer.prime();
}

float Info::calcFps() {
  u32 timeDelta = timer.getTimeDelta();

  if (timeDelta == 0) return -1.0F;

  return 15625.0F / (float)timeDelta;  // PAL
}

}  // Namespace Tyra