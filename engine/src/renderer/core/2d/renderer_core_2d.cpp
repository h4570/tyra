/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/2d/renderer_core_2d.hpp"
#include <dma.h>
#include <draw.h>

namespace Tyra {

RendererCore2D::RendererCore2D() {
  context = 0;
  packets[0] = packet2_create(16, P2_TYPE_NORMAL, P2_MODE_NORMAL, 0);
  packets[1] = packet2_create(16, P2_TYPE_NORMAL, P2_MODE_NORMAL, 0);
  rects[0] = new texrect_t;
  rects[1] = new texrect_t;
}

RendererCore2D::~RendererCore2D() {
  packet2_free(packets[0]);
  packet2_free(packets[1]);
  delete rects[0];
  delete rects[1];
}

const float RendererCore2D::GS_CENTER = 4096.0F;
const float RendererCore2D::SCREEN_CENTER = 4096.0F / 2.0F;

void RendererCore2D::init(RendererSettings* t_settings,
                          clutbuffer_t* t_clutBuffer) {
  settings = t_settings;
  clutBuffer = t_clutBuffer;
}

void RendererCore2D::render(Sprite* sprite,
                            const RendererCoreTextureBuffers& texBuffers,
                            Texture* texture) {
  auto* rect = rects[context];
  float sizeX, sizeY;

  if (sprite->getMode() == MODE_REPEAT) {
    sizeX = sprite->size.x;
    sizeY = sprite->size.y;
  } else {
    sizeX = static_cast<float>(texture->getWidth());
    sizeY = static_cast<float>(texture->getHeight());
  }

  float texS, texT;
  float texMax = texT = texS = sizeX > sizeY ? sizeX : sizeY;

  if (sizeX > sizeY)
    texT = texMax / (sizeX / sizeY);
  else if (sizeY > sizeX)
    texS = texMax / (sizeY / sizeX);

  rect->t0.s = sprite->isFlippedHorizontally() ? texS : 0.0F;
  rect->t0.t = sprite->isFlippedVertically() ? texT : 0.0F;
  rect->t1.s = sprite->isFlippedHorizontally() ? 0.0F : texS;
  rect->t1.t = sprite->isFlippedVertically() ? 0.0F : texT;

  rect->color.r = sprite->color.r;
  rect->color.g = sprite->color.g;
  rect->color.b = sprite->color.b;
  rect->color.a = sprite->color.a;
  rect->color.q = 0;

  rect->v0.x = sprite->position.x;
  rect->v0.y = sprite->position.y;
  // rect->v0.y /= 2.0F;  // interlacing
  rect->v0.z = (u32)-1;

  rect->v1.x = (sprite->size.x * sprite->scale) + sprite->position.x;
  rect->v1.y = (sprite->size.y * sprite->scale) + sprite->position.y;
  // rect->v1.y /= 2.0F;  // interlacing
  rect->v1.z = (u32)-1;

  auto* packet = packets[context];

  packet2_reset(packet, false);
  packet2_update(packet, draw_primitive_xyoffset(packet->base, 0, SCREEN_CENTER,
                                                 SCREEN_CENTER));
  packet2_utils_gif_add_set(packet, 1);
  packet2_utils_gs_add_texbuff_clut(packet, texBuffers.core, clutBuffer);
  draw_enable_blending();
  packet2_update(packet, draw_rect_textured(packet->next, 0, rect));
  packet2_update(packet, draw_primitive_xyoffset(
                             packet->next, 0,
                             SCREEN_CENTER - (settings->getWidth() / 2.0F),
                             SCREEN_CENTER - (settings->getHeight() / 2.0F)));
  draw_disable_blending();
  packet2_update(packet, draw_finish(packet->next));

  dma_channel_wait(DMA_CHANNEL_GIF, 0);
  dma_channel_send_packet2(packet, DMA_CHANNEL_GIF, true);

  context = !context;
}

}  // namespace Tyra
