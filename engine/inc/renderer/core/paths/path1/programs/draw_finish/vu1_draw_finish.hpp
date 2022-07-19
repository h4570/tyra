/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <packet2_utils.h>
#include <string>
#include "../../vu1_program.hpp"

namespace Tyra {

class VU1DrawFinish : public VU1Program {
 public:
  VU1DrawFinish();
  ~VU1DrawFinish();

  std::string getStringName() const;
  void addTag(packet2_t* packet, prim_t* prim) const;
};

}  // namespace Tyra
