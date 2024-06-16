/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/minecraft/programs/mcpip_programs_manager.hpp"

namespace Tyra {

BlockizerProgramsManager::BlockizerProgramsManager() {
  lastProgramName = UndefinedMcpipProgram;
  context = 0;
  vu1BlockData = BlockNotUploaded;

  dma_channel_initialize(DMA_CHANNEL_VIF1, nullptr, 0);

  setProgramsCache();
}

void BlockizerProgramsManager::allocateOnUse() {
  dynamicPackets[0] = packet2_create(800, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
  dynamicPackets[1] = packet2_create(800, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
  staticPacket = packet2_create(2, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
}

void BlockizerProgramsManager::deallocateOnUse() {
  packet2_free(dynamicPackets[0]);
  packet2_free(dynamicPackets[1]);
  packet2_free(staticPacket);
}

BlockizerProgramsManager::~BlockizerProgramsManager() {
  packet2_free(programsPacket);
}

void BlockizerProgramsManager::init(RendererCore* core, prim_t* prim,
                                    lod_t* lod) {
  rendererCore = core;
  culler.init(core, &singleTexBlockData, prim, lod);
  clipper.init(core, &singleTexBlockData, &multiTexBlockData, prim, lod);
}

void BlockizerProgramsManager::setProgramsCache() {
  VU1Program** programs = new VU1Program*[2];
  programs[0] = repo.getProgram(McpipProgramName::McPipCull);
  programs[1] = repo.getProgram(McpipProgramName::McPipAsIs);
  programsPacket =
      rendererCore->getPath1()->createProgramsCache(programs, 2, 0);
  delete[] programs;
}

void BlockizerProgramsManager::uploadVU1Programs() {
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(programsPacket, DMA_CHANNEL_VIF1, true);
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  lastProgramName = UndefinedMcpipProgram;
  vu1BlockData = BlockNotUploaded;
}

void BlockizerProgramsManager::uploadBlock(bool isMulti) {
  if (vu1BlockData != BlockNotUploaded) {
    if (isMulti && vu1BlockData == BlockMultiUploaded) {
      return;
    }
  } else {
    if (!isMulti && vu1BlockData == BlockSingleUploaded) {
      return;
    }
  }

  packet2_reset(staticPacket, false);

  const McpipBlockData& blockData =
      isMulti ? static_cast<McpipBlockData>(multiTexBlockData)
              : static_cast<McpipBlockData>(singleTexBlockData);

  packet2_utils_vu_add_unpack_data(
      staticPacket, VU1_MCPIP_CULL_STATIC_VERTEX_DATA, blockData.comboData,
      blockData.getComboCount(), false);

  packet2_utils_vu_add_end_tag(staticPacket);

  vu1BlockData = isMulti ? BlockMultiUploaded : BlockSingleUploaded;
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(staticPacket, DMA_CHANNEL_VIF1, true);
}

void BlockizerProgramsManager::cullSpam(McpipBlock*** blockPointerArrays,
                                        u32* blockPointerArrayCounts,
                                        u32 blockPointerArraysCount,
                                        RendererCoreTextureBuffers* texBuffers,
                                        const bool& isMulti) {
  uploadBlock(isMulti);

  auto* currentPacket = dynamicPackets[context];

  auto* program = repo.getProgram(McpipProgramName::McPipCull);

  packet2_reset(currentPacket, false);

  for (u32 i = 0; i < blockPointerArraysCount; i++) {
    auto* blockPointerArray = blockPointerArrays[i];
    auto blockPointerArrayCount = blockPointerArrayCounts[i];

    culler.addData(currentPacket, blockPointerArray, blockPointerArrayCount,
                   texBuffers, isMulti);

    addProgram(program);
  }

  sendPacket(program);
}

void BlockizerProgramsManager::cull(McpipBlock** blockPointerArray,
                                    u32 blockPointerArrayCount,
                                    RendererCoreTextureBuffers* texBuffers,
                                    const bool& isMulti) {
  uploadBlock(isMulti);

  auto* currentPacket = dynamicPackets[context];

  auto* program = repo.getProgram(McpipProgramName::McPipCull);

  packet2_reset(currentPacket, false);

  culler.addData(currentPacket, blockPointerArray, blockPointerArrayCount,
                 texBuffers, isMulti);

  addProgram(program);
  sendPacket(program);
}

void BlockizerProgramsManager::clip(McpipBlock* block,
                                    RendererCoreTextureBuffers* texBuffers,
                                    const bool& isMulti) {
  vu1BlockData = BlockNotUploaded;

  auto* currentPacket = dynamicPackets[context];

  auto* program = repo.getProgram(McpipProgramName::McPipAsIs);

  clipper.addData(block, isMulti, texBuffers, currentPacket, context);

  addProgram(program);
  sendPacket(program);
}

void BlockizerProgramsManager::addProgram(McpipProgram* program) {
  auto* currentPacket = dynamicPackets[context];

  if (lastProgramName != program->getName()) {
    packet2_utils_vu_add_start_program(currentPacket,
                                       program->getDestinationAddress());
    lastProgramName = program->getName();
  } else {
    packet2_utils_vu_add_continue_program(currentPacket);
  }
}

void BlockizerProgramsManager::sendPacket(McpipProgram* program) {
  auto* currentPacket = dynamicPackets[context];

  packet2_utils_vu_add_end_tag(currentPacket);

  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_wait(DMA_CHANNEL_GIF, 0);  // Wait for texture. Issue #182.

  // dma_wait_fast(); // This have no impact on performance

  dma_channel_send_packet2(currentPacket, DMA_CHANNEL_VIF1, true);
  context = !context;
}

}  // namespace Tyra
