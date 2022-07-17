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
#include "./mcpip_program.hpp"
#include "cull/mcpip_cull_vu1_program.hpp"
#include "as_is/mcpip_as_is_vu1_program.hpp"

namespace Tyra {

class McpipProgramsRepository {
 public:
  McpipProgramsRepository();
  ~McpipProgramsRepository();

  McpipProgram* getProgram(const McpipProgramName& name);

 private:
  McpipCullVU1Program cull;
  McpipAsIsVU1Program asIs;
};

}  // namespace Tyra
