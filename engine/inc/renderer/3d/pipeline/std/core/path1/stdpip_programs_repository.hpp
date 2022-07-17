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

#include "./programs/as_is/stdpip_as_is_c_vu1_program.hpp"
#include "./programs/as_is/stdpip_as_is_d_vu1_program.hpp"
#include "./programs/as_is/stdpip_as_is_td_vu1_program.hpp"
#include "./programs/as_is/stdpip_as_is_tc_vu1_program.hpp"

#include "./programs/cull/stdpip_cull_c_vu1_program.hpp"
#include "./programs/cull/stdpip_cull_d_vu1_program.hpp"
#include "./programs/cull/stdpip_cull_td_vu1_program.hpp"
#include "./programs/cull/stdpip_cull_tc_vu1_program.hpp"

#include "renderer/core/paths/path1/programs/draw_finish/vu1_draw_finish.hpp"

namespace Tyra {

class StdpipProgramsRepository {
 public:
  StdpipProgramsRepository();
  ~StdpipProgramsRepository();

  StdpipVU1Program* getProgram(const StdpipProgramName& name);

 private:
  StdpipAsIsCVU1Program asIsColor;
  StdpipCullCVU1Program cullColor;
  StdpipAsIsDVU1Program asIsLightingColor;
  StdpipCullDVU1Program cullLightingColor;
  StdpipAsIsTDVU1Program asIsLightingTextureColor;
  StdpipCullTDVU1Program cullLightingTextureColor;
  StdpipAsIsTCVU1Program asIsTextureColor;
  StdpipCullTCVU1Program cullTextureColor;
  VU1DrawFinish drawFinish;
};

}  // namespace Tyra
