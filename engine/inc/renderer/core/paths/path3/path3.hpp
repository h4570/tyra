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

#include <dma.h>
#include <packet2_chain.h>
#include <draw.h>
#include "renderer/core/texture/models/texture.hpp"
#include "renderer/renderer_settings.hpp"
#include "renderer/models/color.hpp"
#include "renderer/core/texture/renderer_core_texture_buffers.hpp"

namespace Tyra {

class Path3 {
 public:
  Path3();
  ~Path3();

  void init(RendererSettings* settings);

  void addDrawFinishTag();
  void clearScreen(zbuffer_t* z, const Color& color);
  void sendTexture(Texture* texture,
                   const RendererCoreTextureBuffers& texBuffers);

 private:
  packet2_t* drawFinishPacket;
  packet2_t* clearScreenPacket;
  packet2_t* texturePacket;
  RendererSettings* settings;
};

}  // namespace Tyra
