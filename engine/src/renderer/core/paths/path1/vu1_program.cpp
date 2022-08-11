/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/paths/path1/vu1_program.hpp"

namespace Tyra {

VU1Program::VU1Program(u32* t_start, u32* t_end) : start(t_start), end(t_end) {
  packetSize = packet2_utils_get_packet_size_for_program(start, end);
  programSize = calculateProgramSize();
}

VU1Program::~VU1Program() {}

const u32& VU1Program::getPacketSize() const { return packetSize; }
const u32& VU1Program::getProgramSize() const { return programSize; }
const u32& VU1Program::getDestinationAddress() const {
  return destinationAddress;
}
void VU1Program::setDestinationAddress(const u32& addr) {
  destinationAddress = addr;
}
u32* VU1Program::getStart() const { return start; }
u32* VU1Program::getEnd() const { return end; }

u32 VU1Program::calculateProgramSize() const {
  u32 count = (getEnd() - getStart()) / 2;
  if (count & 1) count++;

  return count;
}

}  // namespace Tyra
