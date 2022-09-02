/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <timer.h>

namespace Tyra {

class Timer {
 public:
  Timer();
  ~Timer();

  unsigned int getTimeDelta();
  inline void prime() { lastTime = *T3_COUNT; }

 private:
  unsigned int lastTime, time, change;
};

}  // namespace Tyra
