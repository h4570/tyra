/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/paths/path3/path3.hpp"

namespace Tyra {

Path3::Path3() {
  drawFinishPacket = packet2_create(3, P2_TYPE_NORMAL, P2_MODE_CHAIN, false);
  clearScreenPacket = packet2_create(36, P2_TYPE_NORMAL, P2_MODE_CHAIN, false);
  texturePacket = packet2_create(128, P2_TYPE_NORMAL, P2_MODE_CHAIN, false);

  packet2_chain_open_end(drawFinishPacket, 0, 0);
  packet2_update(drawFinishPacket, draw_finish(drawFinishPacket->next));
  packet2_chain_close_tag(drawFinishPacket);
}

Path3::~Path3() {
  packet2_free(drawFinishPacket);
  packet2_free(clearScreenPacket);
  packet2_free(texturePacket);
}

void Path3::init(RendererSettings* t_settings) {
  settings = t_settings;

  dma_channel_initialize(DMA_CHANNEL_GIF, nullptr, 0);

  TYRA_LOG("Path3 initialized");
}

void Path3::sendDrawFinishTag() {
  dma_channel_wait(DMA_CHANNEL_GIF, 0);
  dma_channel_send_packet2(drawFinishPacket, DMA_CHANNEL_GIF, true);
}

void Path3::clearScreen(zbuffer_t* z, const Color& color) {
  packet2_reset(clearScreenPacket, false);
  packet2_chain_open_end(clearScreenPacket, 0, 0);
  packet2_update(clearScreenPacket,
                 draw_disable_tests(clearScreenPacket->next, 0, z));
  packet2_update(
      clearScreenPacket,
      draw_clear(clearScreenPacket->next, 0,
                 2048.0F - (settings->getWidth() / 2),
                 2048.0F - (settings->getHeight() / 2), settings->getWidth(),
                 settings->getHeight(), static_cast<int>(color.r),
                 static_cast<int>(color.g), static_cast<int>(color.b)));
  packet2_update(clearScreenPacket,
                 draw_enable_tests(clearScreenPacket->next, 0, z));
  packet2_update(clearScreenPacket, draw_finish(clearScreenPacket->next));
  packet2_chain_close_tag(clearScreenPacket);
  dma_channel_wait(DMA_CHANNEL_GIF, 0);
  dma_channel_send_packet2(clearScreenPacket, DMA_CHANNEL_GIF, true);
}

void Path3::sendTexture(const Texture* texture,
                        const RendererCoreTextureBuffers& texBuffers) {
  packet2_reset(texturePacket, false);

  packet2_update(
      texturePacket,
      draw_texture_transfer(texturePacket->base, texture->core->data,
                            texture->getWidth(), texture->getHeight(),
                            texture->core->psm, texBuffers.core->address,
                            texBuffers.core->width));

  if (texBuffers.clut != nullptr) {
    auto* clut = texture->clut;
    packet2_update(
        texturePacket,
        draw_texture_transfer(texturePacket->next, clut->data, clut->width,
                              clut->height, clut->psm, texBuffers.clut->address,
                              texBuffers.clut->width));
  }

  packet2_chain_open_cnt(texturePacket, 0, 0, 0);
  packet2_update(texturePacket,
                 draw_texture_wrapping(
                     texturePacket->next, 0,
                     const_cast<texwrap_t*>(texture->getWrapSettings())));
  packet2_chain_close_tag(texturePacket);

  packet2_update(texturePacket, draw_texture_flush(texturePacket->next));
  dma_channel_wait(DMA_CHANNEL_GIF, 0);
  dma_channel_send_packet2(texturePacket, DMA_CHANNEL_GIF, true);
}

}  // namespace Tyra
