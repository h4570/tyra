/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "debug/debug.hpp"
#include "renderer/3d/pipeline/std/core/path1/programs/as_is/stdpip_as_is_c_vu1_program.hpp"

extern u32 StdpipVU1As_Is_C_CodeStart __attribute__((section(".vudata")));
extern u32 StdpipVU1As_Is_C_CodeEnd __attribute__((section(".vudata")));

namespace Tyra {

StdpipAsIsCVU1Program::StdpipAsIsCVU1Program()
    : StdpipVU1Program(StdpipAsIsColor, &StdpipVU1As_Is_C_CodeStart,
                       &StdpipVU1As_Is_C_CodeEnd,
                       ((u64)GIF_REG_RGBAQ) << 0 | ((u64)GIF_REG_XYZ2) << 4, 2,
                       2) {}

StdpipAsIsCVU1Program::~StdpipAsIsCVU1Program() {}

std::string StdpipAsIsCVU1Program::getStringName() const {
  return std::string("As is - C");
}

void StdpipAsIsCVU1Program::addProgramQBufferDataToPacket(
    packet2_t* packet, StdpipQBuffer* qbuffer) const {
  u32 addr = VU1_VERT_DATA_ADDR;

  // Add vertices
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->vertices,
                                   qbuffer->size, true);

  // Add colors
  if (qbuffer->bag->color->single == nullptr) {
    addr += qbuffer->size;
    packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->colors,
                                     qbuffer->size, true);
  }
}

}  // namespace Tyra
