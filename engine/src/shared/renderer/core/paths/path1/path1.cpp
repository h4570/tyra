/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/renderer/core/paths/path1/path1.hpp"

extern unsigned int VU1DrawFinish_CodeStart __attribute__((section(".vudata")));
extern unsigned int VU1DrawFinish_CodeEnd __attribute__((section(".vudata")));

namespace Tyra {

Path1::Path1() {
  doubleBufferPacket = packet2_create(2, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
  drawFinishPacket = packet2_create(10, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
  uploadDrawFinishProgram();
  prepareDrawFinishPacket();
}

void Path1::uploadDrawFinishProgram() {
  int count = (&VU1DrawFinish_CodeEnd - &VU1DrawFinish_CodeStart) / 2;
  if (count & 1) count++;

  drawFinishAddr = 1000 - count;

  packet2_t* packet2 = packet2_create(10, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
  packet2_vif_add_micro_program(packet2, drawFinishAddr,
                                &VU1DrawFinish_CodeStart,
                                &VU1DrawFinish_CodeEnd);
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(packet2, DMA_CHANNEL_VIF1, true);
  packet2_free(packet2);
}

void Path1::addDrawFinishTag(packet2_t* packet) {
  prim_t prim;
  prim.type = PRIM_TRIANGLE;
  prim.shading = PRIM_SHADE_GOURAUD;
  prim.mapping = 1;
  prim.fogging = 0;
  prim.blending = 1;
  prim.antialiasing = 0;
  prim.mapping_type = PRIM_MAP_ST;
  prim.colorfix = PRIM_UNFIXED;

  packet2_utils_vu_open_unpack(packet, 10, true);
  {
    packet2_utils_gif_add_set(packet, 1);
    packet2_utils_gs_add_draw_finish_giftag(packet);
    packet2_utils_gs_add_prim_giftag(packet, &prim, 0,
                                     ((u64)GIF_REG_RGBAQ) << 0, 1, 0);
  }
  packet2_utils_vu_close_unpack(packet);

  packet2_utils_vu_add_start_program(packet, drawFinishAddr);
}

void Path1::sendDrawFinishTag() {
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(drawFinishPacket, DMA_CHANNEL_VIF1, true);
}

Path1::~Path1() {
  packet2_free(doubleBufferPacket);
  packet2_free(drawFinishPacket);
}

void Path1::prepareDrawFinishPacket() {
  addDrawFinishTag(drawFinishPacket);
  packet2_utils_vu_add_end_tag(drawFinishPacket);
}

unsigned int Path1::uploadProgram(VU1Program* program,
                                  const unsigned int& address) {
  // TYRA_LOG("Uploading VU1 program. Size: ", program->getProgramSize(),
  //          ", name: ", program->getStringName(), ", address:", address);

  auto packetSize = program->getPacketSize() + 1;  // + end tag

  packet2_t* packet2 =
      packet2_create(packetSize, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);

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

packet2_t* Path1::createProgramsCache(VU1Program** programs,
                                      const unsigned int& count,
                                      const unsigned int& address) {
  unsigned int packetSize = 1;  // + end tag
  for (unsigned int i = 0; i < count; i++) {
    packetSize += programs[i]->getPacketSize();
  }

  packet2_t* packet2 =
      packet2_create(packetSize, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);

  unsigned int currentAddr = address;
  for (unsigned int i = 0; i < count; i++) {
    programs[i]->setDestinationAddress(currentAddr);
    packet2_vif_add_micro_program(packet2, currentAddr, programs[i]->getStart(),
                                  programs[i]->getEnd());
    currentAddr += programs[i]->getProgramSize() + 1;
  }

  packet2_utils_vu_add_end_tag(packet2);

  return packet2;
}

/** Set double buffer settings */
void Path1::setDoubleBuffer(const unsigned short& startingAddress,
                            const unsigned short& bufferSize) {
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
