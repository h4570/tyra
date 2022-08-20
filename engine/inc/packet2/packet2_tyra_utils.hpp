/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <packet2_utils.h>
#include "math/m4x4.hpp"
#include "renderer/models/color.hpp"

namespace Tyra {

class Packet2TyraUtils {
 public:
  inline static void addUnpackData(packet2_t* packet2,
                                   const u32& t_dest_address,
                                   const void* t_data, const u32& t_size,
                                   const u8& t_use_top) {
    packet2_chain_ref(packet2, t_data, t_size, 0, 0, 0);
    packet2_vif_stcycl(packet2, 0, 0x0101, 0);
    packet2_vif_open_unpack(packet2, P2_UNPACK_V4_32, t_dest_address, t_use_top,
                            0, 1, 0);
    packet2_vif_close_unpack_manual(packet2, t_size);
  }

  inline static void addM4x4(packet2_t* packet2, const M4x4& val) {
    *(reinterpret_cast<M4x4*>(packet2->next)) = val;
    packet2_advance_next(packet2, sizeof(MATRIX));
  }

  inline static void addVec4(packet2_t* packet2, const Vec4& val) {
    *(reinterpret_cast<Vec4*>(packet2->next)) = val;
    packet2_advance_next(packet2, sizeof(VECTOR));
  }

  inline static void addColor(packet2_t* packet2, const Color& val) {
    *(reinterpret_cast<Color*>(packet2->next)) = val;
    packet2_advance_next(packet2, sizeof(VECTOR));
  }
};

}  // namespace Tyra
