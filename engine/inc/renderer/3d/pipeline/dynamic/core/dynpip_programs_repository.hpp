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

#include "debug/debug.hpp"
#include "renderer/core/paths/path1/vu1_program.hpp"
#include "./bag/dynpip_bag.hpp"

#include "./programs/dynpip_c_vu1_program.hpp"
#include "./programs/dynpip_d_vu1_program.hpp"
#include "./programs/dynpip_td_vu1_program.hpp"
#include "./programs/dynpip_tc_vu1_program.hpp"

namespace Tyra {

class DynPipProgramsRepository {
 public:
  DynPipProgramsRepository();
  ~DynPipProgramsRepository();

  DynPipVU1Program* getProgram(const DynPipProgramName& name);
  DynPipVU1Program* getProgramByParams(const bool& isLightingEnabled,
                                       const bool& isTextureEnabled);
  DynPipVU1Program* getProgramByBag(const DynPipBag* bag);

 private:
  DynPipCVU1Program color;
  DynPipDVU1Program dirLights;
  DynPipTDVU1Program textureDirLights;
  DynPipTCVU1Program textureColor;
};

}  // namespace Tyra
