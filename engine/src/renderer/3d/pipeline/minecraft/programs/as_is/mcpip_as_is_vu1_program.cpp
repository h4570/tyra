/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/minecraft/programs/as_is/mcpip_as_is_vu1_program.hpp"

extern u32 VU1BlocksAsIs_CodeStart __attribute__((section(".vudata")));
extern u32 VU1BlocksAsIs_CodeEnd __attribute__((section(".vudata")));

namespace Tyra {

McpipAsIsVU1Program::McpipAsIsVU1Program()
    : McpipProgram(McpipProgramName::McPipAsIs, &VU1BlocksAsIs_CodeStart,
                   &VU1BlocksAsIs_CodeEnd) {}

McpipAsIsVU1Program::~McpipAsIsVU1Program() {}

std::string McpipAsIsVU1Program::getStringName() const {
  return std::string("As is");
}

}  // namespace Tyra
