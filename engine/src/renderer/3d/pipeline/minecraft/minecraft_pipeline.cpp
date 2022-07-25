/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tamtypes.h>
#include "renderer/3d/pipeline/minecraft/minecraft_pipeline.hpp"
#include "thread/threading.hpp"

namespace Tyra {

MinecraftPipeline::MinecraftPipeline() {
  latestMode = UndefinedMcpipProgram;
  spamBuffersCount = 4;
  spamBuffers = new McpipBlock**[spamBuffersCount];
  spamCounts = new u32[spamBuffersCount];
  spammerIndex = 0;
}

MinecraftPipeline::~MinecraftPipeline() {
  if (bbox) {
    delete bbox;
  }
  delete[] spamBuffers;
  delete[] spamCounts;
}

void MinecraftPipeline::init(RendererCore* core) {
  rendererCore = core;
  manager.init(core);

  initBBox();
}

void MinecraftPipeline::onUse() {
  manager.uploadVU1Programs();

  changeMode(McPipCull, true);
}

void MinecraftPipeline::initBBox() {
  const auto& block = manager.getBlockData();
  bbox = new RenderBBox(block.vertices, block.count);
}

void MinecraftPipeline::render(McpipBlock* blocks, const u32& count,
                               Texture* t_tex, const bool& isMulti,
                               const bool& fullClipChecks) {
  auto texBuffers = rendererCore->texture.useTexture(t_tex);
  rendererCore->gs.prim.mapping = 1;

  manager.clearLastProgram();
  std::vector<u32> cullIndexes;

  if (!fullClipChecks) {
    for (u32 i = 0; i < count; i++) cullIndexes.push_back(i);
    cull(blocks, cullIndexes, &texBuffers, true, isMulti);
    return;
  }

  u32 culled = 0, clipped = 0;
  std::vector<u32> clipIndexes;

  for (u32 i = 0; i < count; i++) {
    auto frustum = isInFrustum(blocks[i]);
    if (frustum == CoreBBoxFrustum::IN_FRUSTUM) {
      cullIndexes.push_back(i);
      culled++;
    } else if (frustum == CoreBBoxFrustum::PARTIALLY_IN_FRUSTUM) {
      clipIndexes.push_back(i);
      clipped++;
    }
  }

  if (culled > 0) cull(blocks, cullIndexes, &texBuffers, false, isMulti);
  if (clipped > 0) clip(blocks, clipIndexes, &texBuffers, isMulti);
}

CoreBBoxFrustum MinecraftPipeline::isInFrustum(const McpipBlock& block) const {
  const auto* frustumPlanes = rendererCore->renderer3D.frustumPlanes.getAll();
  return bbox->clipIsInFrustum(frustumPlanes, block.model);
}

void MinecraftPipeline::cull(McpipBlock* blocks,
                             const std::vector<u32>& indexes,
                             RendererCoreTextureBuffers* texBuffers,
                             const bool& isCullOnly, const bool& isMulti) {
  changeMode(McPipCull, false);

  auto maxBlocksPerQBuffer = manager.culler.getMaxBlocksCountPerQBuffer();
  auto partsCount = static_cast<u32>(
      ceil(indexes.size() / static_cast<float>(maxBlocksPerQBuffer)));

  for (u32 i = 0; i < partsCount; i++) {
    u32 subArraySize = i != partsCount - 1
                           ? maxBlocksPerQBuffer
                           : indexes.size() - i * maxBlocksPerQBuffer;

    McpipBlock** blockPointerArray = new McpipBlock*[subArraySize];
    u32 blockPointerArrayCount = 0;
    for (u32 j = 0; j < subArraySize; j++) {
      blockPointerArray[blockPointerArrayCount++] =
          &blocks[indexes[i * maxBlocksPerQBuffer + j]];
    }

    if (isCullOnly) {
      addToSpammer(blockPointerArray, blockPointerArrayCount, texBuffers,
                   isMulti);
    } else {
      manager.cull(blockPointerArray, blockPointerArrayCount, texBuffers,
                   isMulti);
      delete[] blockPointerArray;
    }
  }

  if (isCullOnly) {
    flushSpammer(texBuffers, isMulti);
  }
}

// -- 2nd qbuff = 36 * 3 (ST,RGBA,STQ)
// * 2 (We will have at least 2x more verts after clip)
// + Set, lod, set, clut, prim = 221
// -- 1st qbuff: 500 - 221 = 279
// Tags: 279 - for example 20 = 259
// Vert + ST from EE = 259 / 2 = 129 verts | OK!
void MinecraftPipeline::clip(McpipBlock* blocks,
                             const std::vector<u32>& indexes,
                             RendererCoreTextureBuffers* texBuffers,
                             const bool& isMulti) {
  changeMode(McPipAsIs, false);

  for (u32 i = 0; i < indexes.size(); i++) {
    manager.clip(&blocks[indexes[i]], texBuffers, isMulti);
  }
}

void MinecraftPipeline::changeMode(const McpipProgramName& requestedMode,
                                   const u8& force) {
  if (!force) {
    if (latestMode == requestedMode) return;
  }

  if (requestedMode == McPipCull) {
    manager.culler.configureVU1AndSendStaticData();
    latestMode = McPipCull;
  } else {
    manager.clipper.configureVU1AndSendStaticData();
    latestMode = McPipAsIs;
  }
}

void MinecraftPipeline::addToSpammer(McpipBlock** blockPointerArray,
                                     const u32& count,
                                     RendererCoreTextureBuffers* texBuffers,
                                     const bool& isMulti) {
  spamBuffers[spammerIndex] = blockPointerArray;
  spamCounts[spammerIndex] = count;

  spammerIndex++;

  if (spammerIndex == spamBuffersCount) {
    spammerIndex = 0;

    manager.cullSpam(spamBuffers, spamCounts, spamBuffersCount, texBuffers,
                     isMulti);

    for (u32 i = 0; i < spamBuffersCount; i++) {
      delete[] spamBuffers[i];
    }
  }
}

void MinecraftPipeline::flushSpammer(RendererCoreTextureBuffers* texBuffers,
                                     const bool& isMulti) {
  if (spammerIndex == 0) return;

  manager.cullSpam(spamBuffers, spamCounts, spammerIndex, texBuffers, isMulti);

  for (u32 i = 0; i < spammerIndex; i++) {
    delete[] spamBuffers[i];
  }

  spammerIndex = 0;
}

}  // namespace Tyra
