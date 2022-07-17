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

#include <tamtypes.h>
#include <draw_buffers.h>
#include <vector>
#include "renderer/core/texture/models/texture.hpp"
#include "./renderer_texture_cm_analysis.hpp"
#include "../renderer_core_texture_buffers.hpp"

namespace Tyra {

class RendererTextureCacheManager {
 public:
  RendererTextureCacheManager();
  ~RendererTextureCacheManager();

  void onFrameChange();

  void addRequestedTexture(Texture* texture);

  u32 getTextureIdToDealloc(
      const std::vector<RendererCoreTextureBuffers>& currentAllocs);

 private:
  RendererTextureCMAnalysis statistics;

  void tryAddIdThatNotExistInAnalysis(
      std::vector<u32>* result,
      const std::vector<RendererCoreTextureBuffers>& currentAllocs,
      const std::vector<u32>& analysis);

  void addIdThatExistInAnalysisButIsOnTheBottom(
      std::vector<u32>* result,
      const std::vector<RendererCoreTextureBuffers>& currentAllocs,
      const std::vector<u32>& analysis);
};

}  // namespace Tyra
