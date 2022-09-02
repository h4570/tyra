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

#include <draw_buffers.h>
#include <graph.h>
#include "./models/texture.hpp"
#include "shared/renderer/core/paths/path3/path3.hpp"
#include "shared/renderer/core/gs/renderer_core_gs.hpp"
#include "./renderer_core_texture_buffers.hpp"

namespace Tyra {

class RendererCoreTextureSender {
 public:
  RendererCoreTextureSender();
  ~RendererCoreTextureSender();

  void init(Path3* path3, RendererCoreGS* gs);

  RendererCoreTextureBuffers allocate(const Texture* t_texture);

  void deallocate(const RendererCoreTextureBuffers& texBuffers);

  float getSizeInMB(texbuffer_t* texBuffer);

 private:
  RendererCoreGS* gs;
  Path3* path3;
  TextureBpp getBppByPsm(const unsigned int& psm);
  texbuffer_t* allocateTextureCore(const Texture* t_texture);
  texbuffer_t* allocateTextureClut(const Texture* t_texture);
};

}  // namespace Tyra
