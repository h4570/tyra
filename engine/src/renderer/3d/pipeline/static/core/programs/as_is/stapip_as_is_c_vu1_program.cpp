/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "debug/debug.hpp"
#include "renderer/3d/pipeline/static/core/programs/as_is/stapip_as_is_c_vu1_program.hpp"

extern unsigned int StaPipVU1As_Is_C_CodeStart
    __attribute__((section(".vudata")));
extern unsigned int StaPipVU1As_Is_C_CodeEnd
    __attribute__((section(".vudata")));

namespace Tyra {

StaPipAsIsCVU1Program::StaPipAsIsCVU1Program()
    : StaPipVU1Program(StaPipAsIsColor, &StaPipVU1As_Is_C_CodeStart,
                       &StaPipVU1As_Is_C_CodeEnd,
                       ((u64)GIF_REG_RGBAQ) << 0 | ((u64)GIF_REG_XYZ2) << 4, 2,
                       2) {}

StaPipAsIsCVU1Program::~StaPipAsIsCVU1Program() {}

std::string StaPipAsIsCVU1Program::getStringName() const {
  return std::string("StaPip - As is - C");
}

void StaPipAsIsCVU1Program::addProgramQBufferDataToPacket(
    packet2_t* packet, StaPipQBuffer* qbuffer) const {
  unsigned int addr = VU1_STAPIP_VERT_DATA_ADDR;

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
