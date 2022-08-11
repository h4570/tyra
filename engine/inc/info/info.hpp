/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#pragma once

#include <tamtypes.h>
#include <stddef.h>
#include "time/timer.hpp"
#include "./version.hpp"

namespace Tyra {

class Info {
 public:
  Info();
  ~Info();

  Version version;

  /** Called by engine */
  void update();

  const u32& getFps() const { return fps; };

  /** @return Available RAM in MB */
  float getAvailableRAM();

 private:
  float calcFps();
  void* allocateLargestFreeRAMBlock(size_t* size);
  size_t getFreeRAMSize();

  u8 fpsDelayer;
  u32 fps;
  Timer timer;
};

}  // namespace Tyra
