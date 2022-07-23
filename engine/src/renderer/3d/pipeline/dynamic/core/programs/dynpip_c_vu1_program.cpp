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
#include "renderer/3d/pipeline/dynamic/core/programs/dynpip_c_vu1_program.hpp"

extern u32 DynPipVU1_C_CodeStart __attribute__((section(".vudata")));
extern u32 DynPipVU1_C_CodeEnd __attribute__((section(".vudata")));

namespace Tyra {

DynPipCVU1Program::DynPipCVU1Program()
    : DynPipVU1Program(
          DynPipColor, &DynPipVU1_C_CodeStart, &DynPipVU1_C_CodeEnd,
          ((u64)GIF_REG_RGBAQ) << 0 | ((u64)GIF_REG_XYZ2) << 4, 2, 1) {}

DynPipCVU1Program::~DynPipCVU1Program() {}

std::string DynPipCVU1Program::getStringName() const {
  return std::string("DynPip - C");
}

void DynPipCVU1Program::addProgramQBufferDataToPacket(packet2_t* packet,
                                                      DynPipBag* bag) const {
  u32 addr = VU1_VERT_DATA_ADDR;

  // Add vertices
  packet2_utils_vu_add_unpack_data(packet, addr, bag->verticesFrom, bag->count,
                                   true);
  addr += bag->count;
  packet2_utils_vu_add_unpack_data(packet, addr, bag->verticesTo, bag->count,
                                   true);
}

}  // namespace Tyra
