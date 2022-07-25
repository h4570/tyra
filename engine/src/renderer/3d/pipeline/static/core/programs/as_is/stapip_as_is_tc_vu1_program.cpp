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
#include "renderer/3d/pipeline/static/core/programs/as_is/stapip_as_is_tc_vu1_program.hpp"

extern u32 StaPipVU1As_Is_TC_CodeStart __attribute__((section(".vudata")));
extern u32 StaPipVU1As_Is_TC_CodeEnd __attribute__((section(".vudata")));

namespace Tyra {

StaPipAsIsTCVU1Program::StaPipAsIsTCVU1Program()
    : StaPipVU1Program(StaPipAsIsTextureColor, &StaPipVU1As_Is_TC_CodeStart,
                       &StaPipVU1As_Is_TC_CodeEnd,
                       ((u64)GIF_REG_ST) << 0 | ((u64)GIF_REG_RGBAQ) << 4 |
                           ((u64)GIF_REG_XYZ2) << 8,
                       3, 3) {}

StaPipAsIsTCVU1Program::~StaPipAsIsTCVU1Program() {}

std::string StaPipAsIsTCVU1Program::getStringName() const {
  return std::string("StaPip - As is - TC");
}

void StaPipAsIsTCVU1Program::addProgramQBufferDataToPacket(
    packet2_t* packet, StaPipQBuffer* qbuffer) const {
  u32 addr = VU1_VERT_DATA_ADDR;

  // Add vertices
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->vertices,
                                   qbuffer->size, true);
  addr += qbuffer->size;

  // Add sts
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->sts, qbuffer->size,
                                   true);

  // Add colors
  if (qbuffer->bag->color->single == nullptr) {
    addr += qbuffer->size;
    packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->colors,
                                     qbuffer->size, true);
  }
}

}  // namespace Tyra
