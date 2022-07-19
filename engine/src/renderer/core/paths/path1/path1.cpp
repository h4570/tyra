/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/paths/path1/path1.hpp"

namespace Tyra {

Path1::Path1() {
  doubleBufferPacket = packet2_create(2, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
}
Path1::~Path1() { packet2_free(doubleBufferPacket); }

u32 Path1::uploadProgram(VU1Program* program, const u32& address) {
  // TYRA_LOG("Uploading VU1 program. Size: ", program->getProgramSize(),
  //          ", name: ", program->getStringName(), ", address:", address);

  auto packetSize = program->getPacketSize() + 1;  // + end tag

  packet2_t* packet2 =
      packet2_create(packetSize, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);

  packet2_vif_add_micro_program(packet2, address, program->getStart(),
                                program->getEnd());

  packet2_utils_vu_add_end_tag(packet2);

  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(packet2, DMA_CHANNEL_VIF1, true);
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);

  program->setDestinationAddress(address);

  packet2_free(packet2);

  return address + program->getProgramSize();
}

packet2_t* Path1::createProgramsCache(VU1Program** programs, const u32& count,
                                      const u32& address) {
  u32 packetSize = 1;  // + end tag
  for (u32 i = 0; i < count; i++) {
    packetSize += programs[i]->getPacketSize();
  }

  packet2_t* packet2 =
      packet2_create(packetSize, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);

  u32 currentAddr = address;
  for (u32 i = 0; i < count; i++) {
    programs[i]->setDestinationAddress(currentAddr);
    packet2_vif_add_micro_program(packet2, currentAddr, programs[i]->getStart(),
                                  programs[i]->getEnd());
    currentAddr += programs[i]->getProgramSize() + 1;
  }

  packet2_utils_vu_add_end_tag(packet2);

  return packet2;
}

/** Set double buffer settings */
void Path1::setDoubleBuffer(const u16& startingAddress, const u16& bufferSize) {
  packet2_reset(doubleBufferPacket, false);
  packet2_utils_vu_add_double_buffer(doubleBufferPacket, startingAddress,
                                     bufferSize);

  // TYRA_LOG("VU1 double buffer: starting addr: ", startingAddress,
  //          ", offset: ", startingAddress + bufferSize, ", size: ",
  //          bufferSize);

  packet2_utils_vu_add_end_tag(doubleBufferPacket);
  dma_channel_send_packet2(doubleBufferPacket, DMA_CHANNEL_VIF1, true);
}

}  // namespace Tyra
