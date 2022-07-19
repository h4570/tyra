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
#include "renderer/3d/pipeline/dynamic/core/programs/dynpip_td_vu1_program.hpp"

extern u32 DynPipVU1_TD_CodeStart __attribute__((section(".vudata")));
extern u32 DynPipVU1_TD_CodeEnd __attribute__((section(".vudata")));

namespace Tyra {

DynPipTDVU1Program::DynPipTDVU1Program()
    : DynPipVU1Program(DynPipTextureDirLights, &DynPipVU1_TD_CodeStart,
                       &DynPipVU1_TD_CodeEnd,
                       ((u64)GIF_REG_ST) << 0 | ((u64)GIF_REG_RGBAQ) << 4 |
                           ((u64)GIF_REG_XYZ2) << 8,
                       3, 4) {}

DynPipTDVU1Program::~DynPipTDVU1Program() {}

std::string DynPipTDVU1Program::getStringName() const {
  return std::string("DynPip - TD");
}

void DynPipTDVU1Program::addProgramQBufferDataToPacket(
    packet2_t* packet, DynPipQBuffer* qbuffer) const {
  u32 addr = VU1_VERT_DATA_ADDR;

  // Add vertices
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->verticesFrom,
                                   qbuffer->size, true);
  addr += qbuffer->size;
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->verticesTo,
                                   qbuffer->size, true);
  addr += qbuffer->size;

  // Add sts
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->stsFrom,
                                   qbuffer->size, true);
  addr += qbuffer->size;
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->stsTo, qbuffer->size,
                                   true);
  addr += qbuffer->size;

  // Add normal
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->normalsFrom,
                                   qbuffer->size, true);
  addr += qbuffer->size;
  packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->normalsTo,
                                   qbuffer->size, true);
  addr += qbuffer->size;

  // Add colors
  if (qbuffer->bag->color->singleFrom == nullptr) {
    packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->colorsFrom,
                                     qbuffer->size, true);
    packet2_utils_vu_add_unpack_data(packet, addr, qbuffer->colorsTo,
                                     qbuffer->size, true);
  }
}

}  // namespace Tyra
