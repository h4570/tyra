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

#include <packet2_utils.h>
#include "debug/debug.hpp"
#include "renderer/renderer.hpp"
#include "packet2/packet2_tyra_utils.hpp"
#include "../mcpip_programs_repository.hpp"
#include "../../mcpip_block.hpp"
#include "../../data/mcpip_block_data.hpp"
#include "./mcpip_vu1_as_is_shared_defines.h"
#include "renderer/core/paths/path1/clipper/path1_ee_clip_algorithm.hpp"

namespace Tyra {

class McpipClip {
 public:
  McpipClip();
  ~McpipClip();

  Path1EEClipAlgorithm algorithm;
  Path1EEClipAlgorithmSettings algoSettings;

  void init(RendererCore* core, McpipBlockData* t_singleBlockData,
            McpipBlockData* t_multiBlockData);

  u32 uploadVU1Program(McpipProgramsRepository* repo, const u32& addr);

  void configureVU1AndSendStaticData();

  void addData(McpipBlock* block, const bool& isMulti,
               RendererCoreTextureBuffers* texBuffers, packet2_t* packet,
               const u8& context);

 private:
  RendererCore* rendererCore;
  McpipBlockData* singleBlockData;
  McpipBlockData* multiBlockData;
  packet2_t* staticPacket;
  u16 vu1DBufferSize;

  std::vector<Path1ClipVertex> clippedTriangle;
  std::vector<Path1ClipVertex> inputTriangle;

  Vec4 vertexBuffers[2][108];
  Vec4 texCoordBuffers[2][108];

  void addCorrections(std::vector<Path1ClipVertex>* vertices,
                      McpipBlock* block);
  void moveDataToBuffer(std::vector<Path1ClipVertex>* vertices,
                        const u8& context);
  void addDataToPacket(packet2_t* packet, const u8& context, McpipBlock* block,
                       const int& count,
                       RendererCoreTextureBuffers* texBuffers);

  void initStaticPacket();
  void sendVU1StaticData();
  void setDBufferSize();
};

}  // namespace Tyra
