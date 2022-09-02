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
#include "renderer/3d/pipeline/static/core/programs/as_is/stapip_as_is_d_vu1_program.hpp"

extern unsigned int StaPipVU1As_Is_D_CodeStart
    __attribute__((section(".vudata")));
extern unsigned int StaPipVU1As_Is_D_CodeEnd
    __attribute__((section(".vudata")));

namespace Tyra {

StaPipAsIsDVU1Program::StaPipAsIsDVU1Program()
    : StaPipVU1Program(StaPipAsIsDirLights, &StaPipVU1As_Is_D_CodeStart,
                       &StaPipVU1As_Is_D_CodeEnd,
                       ((u64)GIF_REG_RGBAQ) << 0 | ((u64)GIF_REG_XYZ2) << 4, 2,
                       3) {}

StaPipAsIsDVU1Program::~StaPipAsIsDVU1Program() {}

std::string StaPipAsIsDVU1Program::getStringName() const {
  return std::string("StaPip - As is - D");
}

void StaPipAsIsDVU1Program::addProgramQBufferDataToPacket(
    packet2_t* packet, StaPipQBuffer* qbuffer) const {
  unsigned int addr = VU1_STAPIP_VERT_DATA_ADDR;

  // Add vertices
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->vertices,
                                   qbuffer->size, true);
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
