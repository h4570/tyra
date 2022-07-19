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
#include <vector>
#include "./texture_repository.hpp"
#include "./renderer_core_texture_sender.hpp"
#include "renderer/core/paths/path3/path3.hpp"
#include "./cache_manager/renderer_texture_cache_manager.hpp"
#include "./renderer_core_texture_buffers.hpp"

namespace Tyra {

class RendererCoreTexture {
 public:
  RendererCoreTexture();
  ~RendererCoreTexture();

  clutbuffer_t clut;
  TextureRepository repository;

  RendererCoreTextureBuffers useTexture(Texture* t_tex);

  /** Called by renderer during initialization */
  void init(RendererCoreGS* gs, Path3* path3);

  /** Called by renderer during beginFrame() */
  void onFrameChange();

  /** Called by renderer during rendering */
  void updateClutBuffer(texbuffer_t* clutBuffer);

 private:
  RendererTextureCacheManager cacheManager;
  std::vector<RendererCoreTextureBuffers> currentAllocations;

  void initClut();
  void registerAllocation(const RendererCoreTextureBuffers& t_buffers);
  void unregisterAllocation(const u32& textureId);
  RendererCoreTextureBuffers getAllocatedBuffersByTextureId(const u32& id);

  RendererCoreTextureSender sender;
  Path3* path3;
};

}  // namespace Tyra
