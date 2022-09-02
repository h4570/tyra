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

#include <dma.h>
#include <packet2_utils.h>
#include "shared/debug/debug.hpp"
#include "./vu1_program.hpp"

namespace Tyra {

class Path1 {
 public:
  Path1();
  ~Path1();

  unsigned int uploadProgram(VU1Program* program, const unsigned int& address);

  void sendDrawFinishTag();

  void addDrawFinishTag(packet2_t* packet);

  packet2_t* createProgramsCache(VU1Program** programs,
                                 const unsigned int& count,
                                 const unsigned int& address);

  void setDoubleBuffer(const unsigned short& startingAddress,
                       const unsigned short& bufferSize);

 private:
  void uploadDrawFinishProgram();
  void prepareDrawFinishPacket();

  packet2_t* doubleBufferPacket;
  packet2_t* drawFinishPacket;
  unsigned int drawFinishAddr;
};

}  // namespace Tyra
