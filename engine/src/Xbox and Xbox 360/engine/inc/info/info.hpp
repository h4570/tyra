/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
# André Guilherme <andregui17@outlook.com>
*/

#pragma once
#ifdef OG_XBOX
typedef unsigned char u8;
typedef unsigned int u32;
#elif XBOX_360
#include <stdint.h>
#else
#include <tamtypes.h>
#endif
#include <stddef.h>
#include "time/timer.hpp"
#include "./version.hpp"

namespace Tyra {

class Info {
 public:
  Info();
  ~Info();

  Version version;

  static bool writeLogsToFile;

  /** Called by engine */
  void update();

#ifndef XBOX_360
  const u32& getFps() const { return fps; };
#else
  const uint32_t& getFps() const { return fps; };
#endif
  /** @return Available RAM in MB */
  float getAvailableRAM();

 private:
  float calcFps();
  void* allocateLargestFreeRAMBlock(size_t* size);
  size_t getFreeRAMSize();
#ifdef XBOX_360
  uint8_t fpsDelayer;
  uint32_t fps;
#else
  u8 fpsDelayer;
  u32 fps;
#endif
  Timer timer;
};

}  // namespace Tyra
