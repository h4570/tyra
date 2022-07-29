/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "time/timer.hpp"

namespace Tyra {

Timer::Timer() { prime(); }

Timer::~Timer() {}

u32 Timer::getTimeDelta() {
  time = *T3_COUNT;

  if (time < lastTime)  // The counter has wrapped
    change = time + (65536 - lastTime);
  else
    change = time - lastTime;
  return change;
}

}  // namespace Tyra
