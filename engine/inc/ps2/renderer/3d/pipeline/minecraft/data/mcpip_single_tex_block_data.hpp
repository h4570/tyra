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

#include "debug/debug.hpp"
#include "./mcpip_block_data.hpp"
#include "string"

namespace Tyra {

class McpipSingleTexBlockData : public McpipBlockData {
 public:
  McpipSingleTexBlockData();
  ~McpipSingleTexBlockData();

 private:
  Vec4* tempVerts;
  Vec4* tempTexCoords;
  unsigned int* tempVertFaces;
  unsigned int* tempTexCoordsFaces;

  void allocateTempData();
  void unroll();
  void dellocateTempData();
};

}  // namespace Tyra
