/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/minecraft/programs/cull/mcpip_cull.hpp"

namespace Tyra {

McpipCull::McpipCull() {
  staticPacket = packet2_create(8, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
}

McpipCull::~McpipCull() { packet2_free(staticPacket); }

void McpipCull::init(RendererCore* core, McpipBlockData* t_blockData) {
  blockData = t_blockData;
  rendererCore = core;
  initStaticPacket();
  setDBufferSize();
}

void McpipCull::setDBufferSize() {
  vu1DBufferSize = 1000;                                   // VU1 mem size
  vu1DBufferSize -= VU1_MCPIP_CULL_STATIC_LAST_DATA_ADDR;  // static data

  // Our additional, mini output double buffer at the bottom of VU1 mem
  u16 miniOutputDoubleBufferSize =
      blockData->count * 3 +
      5;  // 36 * (ST, XYZ2, RGBAQ) + Set, lod, set, clut, prim

  vu1DBufferSize -= miniOutputDoubleBufferSize;  // First
  vu1DBufferSize -= miniOutputDoubleBufferSize;  // Second

  TYRA_ASSERT(1000 - (miniOutputDoubleBufferSize * 2) ==
                  VU1_MCPIP_CULL_DYNAMIC_OUTPUT_DOUBLE_BUFF1_ADDR,
              "There is mismatch in output dbuffer1 size. Should be: ",
              1000 - (miniOutputDoubleBufferSize * 2),
              "but is: ", VU1_MCPIP_CULL_DYNAMIC_OUTPUT_DOUBLE_BUFF1_ADDR);

  TYRA_ASSERT(1000 - miniOutputDoubleBufferSize ==
                  VU1_MCPIP_CULL_DYNAMIC_OUTPUT_DOUBLE_BUFF2_ADDR,
              "There is mismatch in output dbuffer2 size. Should be: ",
              1000 - miniOutputDoubleBufferSize,
              "but is: ", VU1_MCPIP_CULL_DYNAMIC_OUTPUT_DOUBLE_BUFF2_ADDR);

  vu1DBufferSize /= 2;  // xtop double buffer
}

u32 McpipCull::uploadVU1Program(McpipProgramsRepository* repo,
                                const u32& addr) {
  auto* program = repo->getProgram(McpipProgramName::McPipCull);
  return rendererCore->renderer3D.uploadVU1Program(program, addr);
}

void McpipCull::configureVU1AndSendStaticData() {
  rendererCore->renderer3D.setVU1DoubleBuffers(
      VU1_MCPIP_CULL_STATIC_LAST_DATA_ADDR, vu1DBufferSize);

  sendVU1StaticData();
}

u32 McpipCull::getMaxBlocksCountPerQBuffer() const {
  u32 result = vu1DBufferSize;
  result -= 1;  // VU1_MCPIP_CULL_DYNAMIC_SCALE_AND_BLOCKS_COUNT;
  result -= 1;  // Lod tag
  result -= 4;  // View proj

  result /= VU1_MCPIP_CULL_QWORDS_PER_BLOCK;

  return result;
}

void McpipCull::initStaticPacket() {
  packet2_utils_vu_open_unpack(staticPacket, VU1_MCPIP_CULL_STATIC_LOD, false);
  {
    packet2_utils_gs_add_lod(staticPacket, &rendererCore->gs.lod);

    packet2_utils_gs_add_prim_giftag(staticPacket, &rendererCore->gs.prim, 36,
                                     ((u64)GIF_REG_ST) << 0 |
                                         ((u64)GIF_REG_RGBAQ) << 4 |
                                         ((u64)GIF_REG_XYZ2) << 8,
                                     3, 0);
    packet2_utils_gif_add_set(staticPacket, 1);

    packet2_add_u32(staticPacket, 0);  // Param1 - Mini double buffer switcher
    packet2_add_u32(staticPacket, 0);
    packet2_add_u32(staticPacket, 0);
    packet2_add_u32(staticPacket,
                    1);  // static 1 - used in VU1's GetDestinationAddress{}
  }
  packet2_utils_vu_close_unpack(staticPacket);

  packet2_utils_vu_add_end_tag(staticPacket);
}

void McpipCull::sendVU1StaticData() {
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(staticPacket, DMA_CHANNEL_VIF1, true);
}

void McpipCull::addData(packet2_t* packet, McpipBlock** blockPointerArray,
                        u32 blockPointerArrayCount,
                        RendererCoreTextureBuffers* texBuffers, bool isMulti) {
  packet2_reset(packet, false);

  rendererCore->texture.updateClutBuffer(texBuffers->clut);

  packet2_utils_vu_open_unpack(
      packet, VU1_MCPIP_CULL_DYNAMIC_SCALE_AND_BLOCKS_COUNT_ADDR, true);
  {
    packet2_add_float(packet, 2048.0F);  // scale
    packet2_add_float(packet, 2048.0F);  // scale
    packet2_add_float(packet,
                      static_cast<float>(0xFFFFFF) / 32.0F);  // scale
    packet2_add_u32(packet, blockPointerArrayCount);          // blocks count

    packet2_utils_gs_add_texbuff_clut(packet, texBuffers->core,
                                      &rendererCore->texture.clut);

    Packet2TyraUtils::addM4x4(packet, rendererCore->renderer3D.getViewProj());
  }
  packet2_utils_vu_close_unpack(packet);

  u32 addr = VU1_MCPIP_CULL_DYNAMIC_BLOCKS_DATA;

  for (u32 i = 0; i < blockPointerArrayCount; i++) {
    packet2_utils_vu_add_unpack_data(packet, addr, blockPointerArray[i],
                                     VU1_MCPIP_CULL_QWORDS_PER_BLOCK, true);
    addr += VU1_MCPIP_CULL_QWORDS_PER_BLOCK;
  }
}

}  // namespace Tyra
