/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/minecraft/programs/cull/mcpip_cull_vu1_program.hpp"

extern u32 VU1BlocksCull_CodeStart __attribute__((section(".vudata")));
extern u32 VU1BlocksCull_CodeEnd __attribute__((section(".vudata")));

namespace Tyra {

McpipCullVU1Program::McpipCullVU1Program()
    : McpipProgram(McpipProgramName::McPipCull, &VU1BlocksCull_CodeStart,
                   &VU1BlocksCull_CodeEnd) {}

McpipCullVU1Program::~McpipCullVU1Program() {}

std::string McpipCullVU1Program::getStringName() const {
  return std::string("McPip - Cull");
}

}  // namespace Tyra
