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

#pragma once
#ifdef OG_XBOX
typedef unsigned int u32;
#define nullptr NULL
#elif XBOX_360 
#include <stdint.h>
#else
#include <tamtypes.h>
#endif
#include <xtl.h>
#include <time.h>



namespace Tyra {

#if defined(XBOX_360) || defined(OG_XBOX)//Microsoft Xbox 360 SDK and Xbox XDK doesn`t support timespec.
typedef struct 
{
  time_t tv_sec;
  long tv_nsec;
}timespec;
#endif

class Threading {
 public:
  Threading();
  ~Threading();

#if defined(XBOX_360)
   static void sleep(const uint32_t& ms);
#else
  static void sleep(const u32& ms);
#endif
  static void sleep(const timespec& tv);
  static void switchThread();
#if defined(XBOX_360) || defined(OG_XBOX)
  static bool nanosleep(const timespec *rqtp, timespec *rmtp);
#endif
 private:
  static timespec tv;
};
}  // namespace Tyra
