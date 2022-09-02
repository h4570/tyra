/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/debug/debug.hpp"
#include "shared/renderer/3d/pipeline/static/core/programs/cull/stapip_cull_d_vu1_program.hpp"

extern unsigned int StaPipVU1Cull_D_CodeStart
    __attribute__((section(".vudata")));
extern unsigned int StaPipVU1Cull_D_CodeEnd __attribute__((section(".vudata")));

namespace Tyra {

StaPipCullDVU1Program::StaPipCullDVU1Program()
    : StaPipVU1Program(StaPipCullDirLights, &StaPipVU1Cull_D_CodeStart,
                       &StaPipVU1Cull_D_CodeEnd,
                       ((u64)GIF_REG_RGBAQ) << 0 | ((u64)GIF_REG_XYZ2) << 4, 2,
                       3) {}

StaPipCullDVU1Program::~StaPipCullDVU1Program() {}

std::string StaPipCullDVU1Program::getStringName() const {
  return std::string("StaPip - Cull - D");
}

void StaPipCullDVU1Program::addProgramQBufferDataToPacket(
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
