/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <tamtypes.h>
#include <timer.h>

namespace Tyra {

class Timer {
 public:
  Timer();
  ~Timer();

  u32 getTimeDelta();
  inline void prime() { lastTime = *T3_COUNT; }

 private:
  u32 lastTime, time, change;
};

}  // namespace Tyra
