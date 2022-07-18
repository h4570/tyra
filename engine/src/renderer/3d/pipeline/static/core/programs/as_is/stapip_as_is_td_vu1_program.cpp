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
#include "renderer/3d/pipeline/static/core/programs/as_is/stapip_as_is_td_vu1_program.hpp"

extern u32 StapipVU1As_Is_TD_CodeStart __attribute__((section(".vudata")));
extern u32 StapipVU1As_Is_TD_CodeEnd __attribute__((section(".vudata")));

namespace Tyra {

StapipAsIsTDVU1Program::StapipAsIsTDVU1Program()
    : StapipVU1Program(StapipAsIsTextureDirLights, &StapipVU1As_Is_TD_CodeStart,
                       &StapipVU1As_Is_TD_CodeEnd,
                       ((u64)GIF_REG_ST) << 0 | ((u64)GIF_REG_RGBAQ) << 4 |
                           ((u64)GIF_REG_XYZ2) << 8,
                       3, 4) {}

StapipAsIsTDVU1Program::~StapipAsIsTDVU1Program() {}

std::string StapipAsIsTDVU1Program::getStringName() const {
  return std::string("As is - LTC");
}

void StapipAsIsTDVU1Program::addProgramQBufferDataToPacket(
    packet2_t* packet, StapipQBuffer* qbuffer) const {
  u32 addr = VU1_VERT_DATA_ADDR;

  // Add vertices
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->vertices,
                                   qbuffer->size, true);
  addr += qbuffer->size;

  // Add sts
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->sts, qbuffer->size,
                                   true);
  addr += qbuffer->size;

  // Add normal
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->normals,
                                   qbuffer->size, true);

  // Add colors
  if (qbuffer->bag->color->single == nullptr) {
    addr += qbuffer->size;
    packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->colors,
                                     qbuffer->size, true);
  }
}

}  // namespace Tyra