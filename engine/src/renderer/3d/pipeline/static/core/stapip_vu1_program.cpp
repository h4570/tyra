/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/static/core/stapip_vu1_program.hpp"

namespace Tyra {

StaPipVU1Program::StaPipVU1Program(const StaPipProgramName& t_name,
                                   unsigned int* t_start, unsigned int* t_end,
                                   const unsigned int& t_reglist,
                                   const unsigned char& t_reglistCount,
                                   const unsigned char& t_elementsPerVertex)
    : VU1Program(t_start, t_end),
      name(t_name),
      reglistCount(t_reglistCount),
      elementsPerVertex(t_elementsPerVertex),
      reglist(t_reglist) {
  packetSize = packet2_utils_get_packet_size_for_program(start, end);
  programSize = calculateProgramSize();
}

StaPipVU1Program::~StaPipVU1Program() {}

const StaPipProgramName& StaPipVU1Program::getName() const { return name; }

unsigned int& StaPipVU1Program::getReglist() { return reglist; }

void StaPipVU1Program::addBufferDataToPacket(packet2_t* packet,
                                             StaPipQBuffer* buffer,
                                             prim_t* prim) {
  addStandardBufferDataToPacket(packet, buffer, prim);
  addProgramQBufferDataToPacket(packet, buffer);
}

void StaPipVU1Program::addStandardBufferDataToPacket(packet2_t* packet,
                                                     StaPipQBuffer* buffer,
                                                     prim_t* prim) {
  if (buffer->bag->texture)
    prim->mapping = 1;
  else
    prim->mapping = 0;

  packet2_utils_vu_open_unpack(packet, 0, true);
  {
    packet2_add_float(packet, 2048.0F);                               // scale
    packet2_add_float(packet, 2048.0F);                               // scale
    packet2_add_float(packet, static_cast<float>(0xFFFFFF) / 32.0F);  // scale
    packet2_add_u32(packet, buffer->size);  // vertex count

    packet2_utils_gs_add_prim_giftag(packet, prim, buffer->size, reglist,
                                     reglistCount, 0);
  }
  packet2_utils_vu_close_unpack(packet);
}

unsigned short StaPipVU1Program::getMaxVertCount(
    const bool& singleColorEnabled, const unsigned short& bufferSize) const {
  unsigned short res = bufferSize - 7;  // 7 because of -> StoreTyraGifTags{}
  unsigned char colorElementsPerVertex =
      singleColorEnabled ? elementsPerVertex - 1 : elementsPerVertex;
  res /= (colorElementsPerVertex + reglistCount);

  // Buffer size = VU1 double buffer size (xtop)
  // QBufferSize = res (it is placed inside VU1)

  // Must be dividable by 3 and the result also dividable by 3. Why?
  // 1st dividable reason - triangle, and packaging system in 3d rendering
  // 2nd dividable reason - subpackaging system. We are splitting packages into
  // 3 subpackages in 3d renderer.
  res = res / 3 / 3;
  res = res * 3 * 3;
  return res;
}

}  // namespace Tyra
