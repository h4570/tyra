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

class MinecraftPipeline : public Renderer3DPipeline {
 public:
  MinecraftPipeline();
  ~MinecraftPipeline();

  void init(RendererCore* core);

  void onUse();

  void render(McpipBlock* blocks, const u32& count, Texture* t_tex,
              const bool& isMulti = false, const bool& noClipChecks = true);

  inline const float& getTextureOffset() const {
    return manager.getTextureOffset();
  }

 private:
  BlockizerProgramsManager manager;
  RendererCore* rendererCore;
  RenderBBox* bbox;
  McpipProgramName latestMode;

  void initBBox();

  void changeMode(const McpipProgramName& requestedMode, const u8& force);

  void cull(McpipBlock* blocks, const std::vector<u32>& indexes,
            RendererCoreTextureBuffers* texBuffers,
            const bool& isMulti = false);

  void clip(McpipBlock* blocks, const std::vector<u32>& indexes,
            RendererCoreTextureBuffers* texBuffers,
            const bool& isMulti = false);

  Tyra::CoreBBoxFrustum isInFrustum(const McpipBlock& block) const;
};

}  // namespace Tyra
