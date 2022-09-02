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
#include "shared/debug/debug.hpp"
#include "shared/renderer/renderer.hpp"
#include "./cull/mcpip_cull.hpp"
#include "./as_is/mcpip_clip.hpp"
#include "./mcpip_programs_repository.hpp"
#include "../data/mcpip_single_tex_block_data.hpp"
#include "../data/mcpip_multi_tex_block_data.hpp"
#include "../mcpip_block.hpp"

namespace Tyra {

enum McpipStaticBlockData {
  BlockNotUploaded,
  BlockMultiUploaded,
  BlockSingleUploaded
};

class BlockizerProgramsManager {
 public:
  BlockizerProgramsManager();
  ~BlockizerProgramsManager();

  McpipCull culler;
  McpipClip clipper;

  void init(RendererCore* core, prim_t* prim, lod_t* lod);

  void uploadVU1Programs();

  void clearLastProgram() { lastProgramName = UndefinedMcpipProgram; }

  void cullSpam(McpipBlock*** blockPointerArrays,
                unsigned int* blockPointerArrayCounts,
                unsigned int blockPointerArraysCount,
                RendererCoreTextureBuffers* texBuffers, const bool& isMulti);

  void cull(McpipBlock** blockPointerArray, unsigned int blockPointerArrayCount,
            RendererCoreTextureBuffers* texBuffers, const bool& isMulti);

  void clip(McpipBlock* block, RendererCoreTextureBuffers* texBuffers,
            const bool& isMulti);

  inline const float& getTextureOffset() const {
    return singleTexBlockData.offset;
  }

  const McpipBlockData& getBlockData() const { return singleTexBlockData; }

  void allocateOnUse();
  void deallocateOnUse();

 private:
  McpipProgramsRepository repo;
  RendererCore* rendererCore;
  McpipSingleTexBlockData singleTexBlockData;
  McpipMultiTexBlockData multiTexBlockData;
  McpipProgramName lastProgramName;

  packet2_t* programsPacket;
  packet2_t* staticPacket;
  packet2_t* dynamicPackets[2];
  unsigned char context;
  McpipStaticBlockData vu1BlockData;

  void setProgramsCache();
  void uploadBlock(bool isMulti);
  void addProgram(McpipProgram* program);
  void sendPacket(McpipProgram* program);
};

}  // namespace Tyra
