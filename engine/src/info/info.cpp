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
#include "strings.h"
#include <stdlib.h>

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

float Info::getAvailableRAM() {
  size_t bits = getFreeRAMSize();
  return bits / 1024.0F / 1024.0F;
}

void* Info::allocateLargestFreeRAMBlock(size_t* Size) {
  size_t s0, s1;
  void* p;

  s0 = ~(size_t)0 ^ (~(size_t)0 >> 1);

  while (s0 && (p = malloc(s0)) == NULL) s0 >>= 1;

  if (p) free(p);

  s1 = s0 >> 1;

  while (s1) {
    if ((p = malloc(s0 + s1)) != NULL) {
      s0 += s1;
      free(p);
    }
    s1 >>= 1;
  }

  while (s0 && (p = malloc(s0)) == NULL) s0 ^= s0 & -s0;

  *Size = s0;
  return p;
}

size_t Info::getFreeRAMSize() {
  size_t total = 0;
  void* pFirst = NULL;
  void* pLast = NULL;

  for (;;) {
    size_t largest;
    void* p = allocateLargestFreeRAMBlock(&largest);

    if (largest < sizeof(void*)) {
      if (p != NULL) free(p);
      break;
    }

    *(void**)p = NULL;

    total += largest;

    if (pFirst == NULL) pFirst = p;

    if (pLast != NULL) *(void**)pLast = p;

    pLast = p;
  }

  while (pFirst != NULL) {
    void* p = *(void**)pFirst;
    free(pFirst);
    pFirst = p;
  }

  return total;
}

}  // Namespace Tyra