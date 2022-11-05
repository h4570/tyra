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

#if defined(OG_XBOX) || defined(XBOX_360)
#include <time.h>
#include <sys/types.h>
#ifdef XBOX_360
#include <stdint.h>
#else
typedef unsigned int u32;
typedef u32 uint32_t;
#endif
#endif

#if defined(XBOX_360) || defined(OG_XBOX)  

#else
#include <tamtypes.h>
#include <timer.h>
#endif


namespace Tyra {

class Timer {
 public:
  Timer();
  ~Timer();
#if XBOX_360
  uint32_t getTimeDelta();
#else
  u32 getTimeDelta();
#endif
  inline void prime() 
  { 
#if defined(OG_XBOX) || defined(XBOX_360)
	  lastTime = CLOCKS_PER_SEC;
#else
	  lastTime = *T3_COUNT; 
#endif  
  }
 private:
#ifdef XBOX_360
  uint32_t lastTime, time, change;
#else
  u32 lastTime, time, change;
#endif
};

}  // namespace Tyra
