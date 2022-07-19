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
#include "./mcpip_block_data.hpp"
#include "string"

namespace Tyra {

class McpipMultiTexBlockData : public McpipBlockData {
 public:
  McpipMultiTexBlockData();
  ~McpipMultiTexBlockData();

 private:
  Vec4* tempVerts;
  Vec4* tempTexCoords;
  u32* tempVertFaces;
  u32* tempTexCoordsFaces;

  void allocateTempData();
  void unroll();
  void dellocateTempData();
};

}  // namespace Tyra
