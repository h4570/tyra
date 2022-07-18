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
#include "renderer/3d/pipeline/static/core/programs/cull/stapip_cull_d_vu1_program.hpp"

extern u32 StapipVU1Cull_D_CodeStart __attribute__((section(".vudata")));
extern u32 StapipVU1Cull_D_CodeEnd __attribute__((section(".vudata")));

namespace Tyra {

StapipCullDVU1Program::StapipCullDVU1Program()
    : StapipVU1Program(StapipCullDirLights, &StapipVU1Cull_D_CodeStart,
                       &StapipVU1Cull_D_CodeEnd,
                       ((u64)GIF_REG_RGBAQ) << 0 | ((u64)GIF_REG_XYZ2) << 4, 2,
                       3) {}

StapipCullDVU1Program::~StapipCullDVU1Program() {}

std::string StapipCullDVU1Program::getStringName() const {
  return std::string("Cull - LC");
}

void StapipCullDVU1Program::addProgramQBufferDataToPacket(
    packet2_t* packet, StapipQBuffer* qbuffer) const {
  u32 addr = VU1_VERT_DATA_ADDR;

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
