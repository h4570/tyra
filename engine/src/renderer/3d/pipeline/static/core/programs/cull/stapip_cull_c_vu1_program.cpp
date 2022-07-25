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
#include "renderer/3d/pipeline/static/core/programs/cull/stapip_cull_c_vu1_program.hpp"

extern u32 StaPipVU1Cull_C_CodeStart __attribute__((section(".vudata")));
extern u32 StaPipVU1Cull_C_CodeEnd __attribute__((section(".vudata")));

namespace Tyra {

StaPipCullCVU1Program::StaPipCullCVU1Program()
    : StaPipVU1Program(
          StaPipCullColor, &StaPipVU1Cull_C_CodeStart, &StaPipVU1Cull_C_CodeEnd,
          ((u64)GIF_REG_RGBAQ) << 0 | ((u64)GIF_REG_XYZ2) << 4, 2, 2) {}

StaPipCullCVU1Program::~StaPipCullCVU1Program() {}

std::string StaPipCullCVU1Program::getStringName() const {
  return std::string("StaPip - Cull - C");
}

void StaPipCullCVU1Program::addProgramQBufferDataToPacket(
    packet2_t* packet, StaPipQBuffer* qbuffer) const {
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
