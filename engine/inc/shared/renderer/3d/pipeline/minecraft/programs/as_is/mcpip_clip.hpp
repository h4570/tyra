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
#include "shared/packet2/packet2_tyra_utils.hpp"
#include "../mcpip_programs_repository.hpp"
#include "../../mcpip_block.hpp"
#include "../../data/mcpip_block_data.hpp"
#include "./mcpip_vu1_as_is_shared_defines.h"
#include "shared/renderer/core/3d/clipper/planes_clip_algorithm.hpp"

namespace Tyra {

class McpipClip {
 public:
  McpipClip();
  ~McpipClip();

  PlanesClipAlgorithm algorithm;
  EEClipAlgorithmSettings algoSettings;

  void init(RendererCore* core, McpipBlockData* t_singleBlockData,
            McpipBlockData* t_multiBlockData, prim_t* prim, lod_t* lod);

  unsigned int uploadVU1Program(McpipProgramsRepository* repo,
                                const unsigned int& addr);

  void configureVU1AndSendStaticData();

  void addData(McpipBlock* block, const bool& isMulti,
               RendererCoreTextureBuffers* texBuffers, packet2_t* packet,
               const unsigned char& context);

 private:
  prim_t* prim;
  lod_t* lod;

  RendererCore* rendererCore;
  McpipBlockData* singleBlockData;
  McpipBlockData* multiBlockData;
  packet2_t* staticPacket;
  unsigned short vu1DBufferSize;

  Vec4 inputVerts[3];
  PlanesClipVertexPtrs inputTriangle[3];
  PlanesClipVertex clippedTriangle[9];

  Vec4 vertexBuffers[2][108];
  Vec4 texCoordBuffers[2][108];

  void addCorrections(std::vector<PlanesClipVertex>* vertices,
                      McpipBlock* block);
  void moveDataToBuffer(std::vector<PlanesClipVertex>* vertices,
                        const unsigned char& context);
  void addDataToPacket(packet2_t* packet, const unsigned char& context,
                       McpipBlock* block, const int& count,
                       RendererCoreTextureBuffers* texBuffers);

  void initStaticPacket();
  void sendVU1StaticData();
  void setDBufferSize();
};

}  // namespace Tyra
