/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022-2022, tyra - https://github.com/h4570/tyrav2
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "info/info.hpp"
#include "strings.h"
#include <stdlib.h>

namespace Tyra {

bool Info::writeLogsToFile = false;

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
  unsigned int timeDelta = timer.getTimeDelta();

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

  while (s0 && (p = malloc(s0)) == nullptr) s0 >>= 1;

  if (p) free(p);

  s1 = s0 >> 1;

  while (s1) {
    if ((p = malloc(s0 + s1)) != nullptr) {
      s0 += s1;
      free(p);
    }
    s1 >>= 1;
  }

  while (s0 && (p = malloc(s0)) == nullptr) s0 ^= s0 & -s0;

  *Size = s0;
  return p;
}

size_t Info::getFreeRAMSize() {
  size_t total = 0;
  void* pFirst = nullptr;
  void* pLast = nullptr;

  for (;;) {
    size_t largest;
    void* p = allocateLargestFreeRAMBlock(&largest);

    if (largest < sizeof(void*)) {
      if (p != nullptr) free(p);
      break;
    }

    *(void**)p = nullptr;

    total += largest;

    if (pFirst == nullptr) pFirst = p;

    if (pLast != nullptr) *(void**)pLast = p;

    pLast = p;
  }

  while (pFirst != nullptr) {
    void* p = *(void**)pFirst;
    free(pFirst);
    pFirst = p;
  }

  return total;
}

}  // Namespace Tyra