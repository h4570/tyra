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

#include "time/timer.hpp"

namespace Tyra {

Timer::Timer() { prime(); }

Timer::~Timer() {}
#ifdef XBOX_360 
uint32_t Timer::getTimeDelta() {
  time = CLOCKS_PER_SEC;

  if (time < lastTime)  // The counter has wrapped
    change = time + (65536 - lastTime);
  else
    change = time - lastTime;
  return change;
}
#else
u32 Timer::getTimeDelta() {
#if defined(OG_XBOX)
	time = CLOCKS_PER_SEC;
#else
    time = *T3_COUNT;
#endif
  if (time < lastTime)  // The counter has wrapped
    change = time + (65536 - lastTime);
  else
    change = time - lastTime;
  return change;
}
#endif
}  // namespace Tyra
