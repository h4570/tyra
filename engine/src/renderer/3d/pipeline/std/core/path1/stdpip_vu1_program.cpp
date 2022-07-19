/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/std/core/path1/stdpip_vu1_program.hpp"

namespace Tyra {

StdpipVU1Program::StdpipVU1Program(const StdpipProgramName& t_name,
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

StdpipVU1Program::~StdpipVU1Program() {}

const StdpipProgramName& StdpipVU1Program::getName() const { return name; }

u32& StdpipVU1Program::getReglist() { return reglist; }

void StdpipVU1Program::addBufferDataToPacket(packet2_t* packet,
                                             StdpipQBuffer* buffer,
                                             prim_t* prim) {
  addStandardBufferDataToPacket(packet, buffer, prim);
  addProgramQBufferDataToPacket(packet, buffer);
}

void StdpipVU1Program::addStandardBufferDataToPacket(packet2_t* packet,
                                                     StdpipQBuffer* buffer,
                                                     prim_t* prim) {
  if (buffer->bag->texture)
    prim->mapping = 1;
  else
    prim->mapping = 0;

  packet2_utils_vu_open_unpack(packet, 0, true);
  {
    packet2_add_float(packet, 2048.0F);  // scale
    packet2_add_float(packet, 2048.0F);  // scale
    packet2_add_float(packet,
                      static_cast<float>(0xFFFFFF) / 32.0F);  // scale
    packet2_add_u32(packet, buffer->size);                    // vertex count

    packet2_utils_gs_add_prim_giftag(packet, prim, buffer->size, reglist,
                                     reglistCount, 0);
  }
  packet2_utils_vu_close_unpack(packet);
}

u16 StdpipVU1Program::getMaxVertCount(const bool& singleColorEnabled,
                                      const u16& bufferSize) const {
  u16 res = bufferSize - 4;
  u8 colorElementsPerVertex =
      singleColorEnabled ? (elementsPerVertex - 1) : elementsPerVertex;
  res /= (colorElementsPerVertex + reglistCount);

  // Buffer size = VU1 double buffer size (xtop)
  // res = qbuffer size (directly inside VU1)

  // Must be dividable by 3 and the result also dividable by 3. Why?
  // 1st dividable reason - triangle, and packaging system in 3d rendering
  // 2nd dividable reason - subpackaging system. We are splitting packages into
  // 3 subpackages in 3d renderer.
  res = res / 3 / 3;
  res = res * 3 * 3;
  return res;
}

}  // namespace Tyra
