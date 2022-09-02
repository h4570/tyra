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
#include "debug/debug.hpp"
#include "renderer/renderer.hpp"
#include "packet2/packet2_tyra_utils.hpp"
#include "../mcpip_programs_repository.hpp"
#include "../../mcpip_block.hpp"
#include "../../data/mcpip_block_data.hpp"
#include "./mcpip_vu1_cull_shared_defines.h"

namespace Tyra {

class McpipCull {
 public:
  McpipCull();
  ~McpipCull();

  void init(RendererCore* core, McpipBlockData* t_blockData, prim_t* prim,
            lod_t* lod);

  unsigned int uploadVU1Program(McpipProgramsRepository* repo,
                                const unsigned int& addr);

  void configureVU1AndSendStaticData();

  unsigned int getMaxBlocksCountPerQBuffer() const;

  void addData(packet2_t* packet, McpipBlock** blockPointerArray,
               unsigned int blockPointerArrayCount,
               RendererCoreTextureBuffers* texBuffers, bool isMulti);

 private:
  prim_t* prim;
  lod_t* lod;
  RendererCore* rendererCore;
  unsigned short vu1DBufferSize;
  McpipBlockData* blockData;
  packet2_t* staticPacket;

  void initStaticPacket();
  void sendVU1StaticData();
  void setDBufferSize();
};

}  // namespace Tyra
