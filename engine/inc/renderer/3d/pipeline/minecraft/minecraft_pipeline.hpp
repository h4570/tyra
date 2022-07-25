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

#include "./mcpip_block.hpp"
#include "./programs/mcpip_programs_manager.hpp"
#include "renderer/3d/pipeline/renderer_3d_pipeline.hpp"
#include "renderer/core/3d/bbox/core_bbox_frustum.hpp"
#include "renderer/core/3d/bbox/render_bbox.hpp"

namespace Tyra {

/**
 * Pipeline specialized in fast rendering of voxels
 * Supports:
 * - Full "against each plane" clipping
 * - Frustum culling (on fullClipChecks = true)
 */
class MinecraftPipeline : public Renderer3DPipeline {
 public:
  MinecraftPipeline();
  ~MinecraftPipeline();

  void setRenderer(RendererCore* core);

  void onUse();
  void onUseEnd();

  /**
   * @brief Render voxels
   *
   * @param blocks blocks to render
   * @param count number of blocks to render
   * @param t_tex texture to use
   * @param isMulti is this a 6 texture voxel?
   * @param fullClipChecks false = faster, simple PS2 clipping. True = slower
   * "against each plane" clipping.
   */
  void render(McpipBlock* blocks, const u32& count, Texture* t_tex,
              const bool& isMulti = false, const bool& fullClipChecks = false);

  inline const float& getTextureOffset() const {
    return manager.getTextureOffset();
  }

  const McpipBlockData& getBlockData() const { return manager.getBlockData(); }

 private:
  BlockizerProgramsManager manager;
  RendererCore* rendererCore;
  RenderBBox* bbox;
  McpipProgramName latestMode;

  McpipBlock*** spamBuffers;
  u32* spamCounts;
  u32 spamBuffersCount;
  u32 spammerIndex;

  void initBBox();

  void changeMode(const McpipProgramName& requestedMode, const u8& force);

  void cull(McpipBlock* blocks, const std::vector<u32>& indexes,
            RendererCoreTextureBuffers* texBuffers, const bool& isCullOnly,
            const bool& isMulti = false);

  void clip(McpipBlock* blocks, const std::vector<u32>& indexes,
            RendererCoreTextureBuffers* texBuffers,
            const bool& isMulti = false);

  Tyra::CoreBBoxFrustum isInFrustum(const McpipBlock& block) const;

  void addToSpammer(McpipBlock** blockPointerArray, const u32& count,
                    RendererCoreTextureBuffers* texBuffers,
                    const bool& isMulti);
  void flushSpammer(RendererCoreTextureBuffers* texBuffers,
                    const bool& isMulti);
};

}  // namespace Tyra
