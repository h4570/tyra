/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/dynamic/core/dynpip_vu1_program.hpp"

namespace Tyra {

DynPipVU1Program::DynPipVU1Program(const DynPipProgramName& t_name,
                                   u32* t_start, u32* t_end,
                                   const u32& t_reglist,
                                   const u8& t_reglistCount,
                                   const u8& t_elementsPerVertex)
    : VU1Program(t_start, t_end),
      name(t_name),
      reglistCount(t_reglistCount),
      elementsPerVertex(t_elementsPerVertex),
      reglist(t_reglist) {
  packetSize = packet2_utils_get_packet_size_for_program(start, end);
  programSize = calculateProgramSize();
}

DynPipVU1Program::~DynPipVU1Program() {}

const DynPipProgramName& DynPipVU1Program::getName() const { return name; }

u32& DynPipVU1Program::getReglist() { return reglist; }

void DynPipVU1Program::addBufferDataToPacket(packet2_t* packet, DynPipBag* bag,
                                             prim_t* prim) {
  addStandardBufferDataToPacket(packet, bag, prim);
  addProgramQBufferDataToPacket(packet, bag);
}

void DynPipVU1Program::addStandardBufferDataToPacket(packet2_t* packet,
                                                     DynPipBag* bag,
                                                     prim_t* prim) {
  if (bag->texture)
    prim->mapping = 1;
  else
    prim->mapping = 0;

  packet2_utils_vu_open_unpack(packet, 0, true);
  {
    packet2_add_float(packet, 2048.0F);                               // scale
    packet2_add_float(packet, 2048.0F);                               // scale
    packet2_add_float(packet, static_cast<float>(0xFFFFFF) / 32.0F);  // scale
    packet2_add_u32(packet, bag->count);  // vertex count

    packet2_utils_gs_add_prim_giftag(packet, prim, bag->count, reglist,
                                     reglistCount, 0);
  }
  packet2_utils_vu_close_unpack(packet);
}

u16 DynPipVU1Program::getMaxVertCount(const u16& bufferSize) const {
  u16 res = bufferSize - 7;  // 7 because of -> StoreTyraGifTags{}
  res /= (elementsPerVertex + reglistCount);

  // Buffer size = VU1 double buffer size (xtop)
  // QBufferSize = res (it is placed inside VU1)

  // Animation = 2 verts for final vert
  res = res / 2;

  // Triangle = 3 verts
  res = res / 3;
  res = res * 3;
  return res;
}

}  // namespace Tyra
