/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <dma.h>
#include <packet2_utils.h>
#include "debug/debug.hpp"
#include "./vu1_program.hpp"

namespace Tyra {

class Path1 {
 public:
  Path1();
  ~Path1();

  u32 uploadProgram(VU1Program* program, const u32& address);

  packet2_t* createProgramsCache(VU1Program** programs, const u32& count,
                                 const u32& address);

  void setDoubleBuffer(const u16& startingAddress, const u16& bufferSize);

 private:
  packet2_t* doubleBufferPacket;
};

}  // namespace Tyra
