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

#include "./programs/as_is/stapip_as_is_c_vu1_program.hpp"
#include "./programs/as_is/stapip_as_is_d_vu1_program.hpp"
#include "./programs/as_is/stapip_as_is_td_vu1_program.hpp"
#include "./programs/as_is/stapip_as_is_tc_vu1_program.hpp"

#include "./programs/cull/stapip_cull_c_vu1_program.hpp"
#include "./programs/cull/stapip_cull_d_vu1_program.hpp"
#include "./programs/cull/stapip_cull_td_vu1_program.hpp"
#include "./programs/cull/stapip_cull_tc_vu1_program.hpp"

#include "renderer/core/paths/path1/programs/draw_finish/vu1_draw_finish.hpp"

namespace Tyra {

class StaPipProgramsRepository {
 public:
  StaPipProgramsRepository();
  ~StaPipProgramsRepository();

  StaPipVU1Program* getProgram(const StaPipProgramName& name);

 private:
  StaPipAsIsCVU1Program asIsColor;
  StaPipCullCVU1Program cullColor;
  StaPipAsIsDVU1Program asIsLightingColor;
  StaPipCullDVU1Program cullLightingColor;
  StaPipAsIsTDVU1Program asIsLightingTextureColor;
  StaPipCullTDVU1Program cullLightingTextureColor;
  StaPipAsIsTCVU1Program asIsTextureColor;
  StaPipCullTCVU1Program cullTextureColor;
  VU1DrawFinish drawFinish;
};

}  // namespace Tyra
