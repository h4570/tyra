/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#pragma once

#include <tamtypes.h>
#include "time/timer.hpp"

namespace Tyra {

class Info {
 public:
  Info();
  ~Info();

  /** Called by engine */
  void update();

  const u32& getFps() const { return fps; };

 private:
  float calcFps();

  u8 fpsDelayer;
  u32 fps;
  Timer timer;
};

}  // namespace Tyra
