/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/renderer/core/paths/path1/vu1_program.hpp"

namespace Tyra {

VU1Program::VU1Program(unsigned int* t_start, unsigned int* t_end)
    : start(t_start), end(t_end) {
  packetSize = packet2_utils_get_packet_size_for_program(start, end);
  programSize = calculateProgramSize();
}

VU1Program::~VU1Program() {}

const unsigned int& VU1Program::getPacketSize() const { return packetSize; }
const unsigned int& VU1Program::getProgramSize() const { return programSize; }
const unsigned int& VU1Program::getDestinationAddress() const {
  return destinationAddress;
}
void VU1Program::setDestinationAddress(const unsigned int& addr) {
  destinationAddress = addr;
}
unsigned int* VU1Program::getStart() const { return start; }
unsigned int* VU1Program::getEnd() const { return end; }

unsigned int VU1Program::calculateProgramSize() const {
  unsigned int count = (getEnd() - getStart()) / 2;
  if (count & 1) count++;

  return count;
}

}  // namespace Tyra
