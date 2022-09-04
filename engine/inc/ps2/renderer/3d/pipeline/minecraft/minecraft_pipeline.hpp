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
  void onFrameEnd();

  /**
   * @brief Render voxels
   *
   * @param blocks array of McpipBlock pointers
   * @param t_tex texture to use
   * @param isMulti is this a 6 texture voxel?
   * @param fullClipChecks false = faster, simple PS2 clipping. True = slower,
   * experimental "against each plane" clipping.
   */
  void render(std::vector<McpipBlock*> blocks, const Texture* t_tex,
              const bool& isMulti = false, const bool& fullClipChecks = false);

  inline const float& getTextureOffset() const {
    return manager.getTextureOffset();
  }

  const McpipBlockData& getBlockData() const { return manager.getBlockData(); }

 private:
  void setPrim();
  void setLod();

  prim_t prim;
  lod_t lod;

  BlockizerProgramsManager manager;
  RendererCore* rendererCore;
  RenderBBox* bbox;
  McpipProgramName latestMode;

  McpipBlock*** spamBuffers;
  unsigned int* spamCounts;
  unsigned int spamBuffersCount;
  unsigned int spammerIndex;

  void initBBox();

  void changeMode(const McpipProgramName& requestedMode,
                  const unsigned char& force);

  void cull(std::vector<McpipBlock*> blocks,
            const std::vector<unsigned int>& indexes,
            RendererCoreTextureBuffers* texBuffers, const bool& isCullOnly,
            const bool& isMulti = false);

  void clip(std::vector<McpipBlock*> blocks,
            const std::vector<unsigned int>& indexes,
            RendererCoreTextureBuffers* texBuffers,
            const bool& isMulti = false);

  Tyra::CoreBBoxFrustum isInFrustum(const McpipBlock& block) const;

  void addToSpammer(McpipBlock** blockPointerArray, const unsigned int& count,
                    RendererCoreTextureBuffers* texBuffers,
                    const bool& isMulti);
  void flushSpammer(RendererCoreTextureBuffers* texBuffers,
                    const bool& isMulti);
};

}  // namespace Tyra
