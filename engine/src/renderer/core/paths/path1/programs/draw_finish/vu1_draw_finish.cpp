/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "debug/debug.hpp"
#include "renderer/core/paths/path1/programs/draw_finish/vu1_draw_finish.hpp"

extern u32 VU1DrawFinish_CodeStart __attribute__((section(".vudata")));
extern u32 VU1DrawFinish_CodeEnd __attribute__((section(".vudata")));

namespace Tyra {

VU1DrawFinish::VU1DrawFinish()
    : VU1Program(&VU1DrawFinish_CodeStart, &VU1DrawFinish_CodeEnd) {}

VU1DrawFinish::~VU1DrawFinish() {}

std::string VU1DrawFinish::getStringName() const {
  return std::string("Draw finish");
}

void VU1DrawFinish::addTag(packet2_t* packet, prim_t* prim) const {
  packet2_utils_vu_open_unpack(packet, 0, true);
  packet2_utils_gs_add_draw_finish_giftag(packet);
  packet2_utils_gs_add_prim_giftag(
      packet, prim, 1, ((u64)GIF_REG_RGBAQ) << 0 | ((u64)GIF_REG_XYZ2) << 4, 2,
      0);
  packet2_utils_vu_close_unpack(packet);
}

}  // namespace Tyra
