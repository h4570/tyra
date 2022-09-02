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

#include <packet2_utils.h>
#include <string>
#include "shared/debug/debug.hpp"
#include "shared/renderer/3d/pipeline/minecraft/programs/mcpip_program.hpp"

namespace Tyra {

class McpipAsIsVU1Program : public McpipProgram {
 public:
  McpipAsIsVU1Program();
  ~McpipAsIsVU1Program();

  std::string getStringName() const;
};

}  // namespace Tyra
