/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <packet2_utils.h>
#include <string>
#include "../dynpip_vu1_program.hpp"

namespace Tyra {

class DynPipTDVU1Program : public DynPipVU1Program {
 public:
  DynPipTDVU1Program();
  ~DynPipTDVU1Program();

  std::string getStringName() const;
  void addProgramQBufferDataToPacket(packet2_t* packet, DynPipBag* bag) const;
};

}  // namespace Tyra
