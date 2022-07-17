/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/minecraft/programs/mcpip_program.hpp"

namespace Tyra {

McpipProgram::McpipProgram(const McpipProgramName& t_name, u32* t_start,
                           u32* t_end)
    : VU1Program(t_start, t_end), name(t_name) {}

McpipProgram::~McpipProgram() {}

const McpipProgramName& McpipProgram::getName() const { return name; }

}  // namespace Tyra
