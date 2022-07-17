/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/minecraft/programs/mcpip_programs_repository.hpp"

namespace Tyra {

McpipProgramsRepository::McpipProgramsRepository() {}

McpipProgramsRepository::~McpipProgramsRepository() {}

McpipProgram* McpipProgramsRepository::getProgram(
    const McpipProgramName& name) {
  switch (name) {
    case McpipProgramName::McPipCull:
      return &cull;
    case McpipProgramName::McPipAsIs:
      return &asIs;

    default:
      TYRA_TRAP("Unknown VU1 program name");
      return &cull;
  }
}

}  // namespace Tyra
