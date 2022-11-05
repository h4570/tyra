/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/minecraft/data/mcpip_block_data.hpp"

namespace Tyra {

McpipBlockData::McpipBlockData() {
  vertices = nullptr;
  textureCoords = nullptr;
  comboData = nullptr;
  offset = 0.0F;
  count = 0;
}

McpipBlockData::~McpipBlockData() {}

}  // namespace Tyra
