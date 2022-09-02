/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "./mcpip_program_name.hpp"
#include "shared/renderer/core/paths/path1/vu1_program.hpp"

namespace Tyra {

class McpipProgram : public VU1Program {
 public:
  McpipProgram(const McpipProgramName& name, unsigned int* start,
               unsigned int* end);
  ~McpipProgram();

  const McpipProgramName& getName() const;

 protected:
  McpipProgramName name;
};

}  // namespace Tyra
