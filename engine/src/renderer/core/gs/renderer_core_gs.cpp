/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <dma.h>
#include <tamtypes.h>
#include <draw.h>
#include <graph.h>
#include <gs_privileged.h>
#include <gs_psm.h>
#include <packet2_utils.h>
#include "debug/debug.hpp"
#include "renderer/core/gs/renderer_core_gs.hpp"

namespace Tyra {

RendererCoreGS::RendererCoreGS() {
  context = 0;
  currentField = 0;
}

RendererCoreGS::~RendererCoreGS() {
  if (flipPacket) {
    packet2_free(flipPacket);
  }
  if (zTestPacket) {
    packet2_free(zTestPacket);
  }
}

void RendererCoreGS::init(RendererSettings* t_settings) {
  settings = t_settings;

  initChannels();
  flipPacket = packet2_create(4, P2_TYPE_UNCACHED_ACCL, P2_MODE_NORMAL, 0);
  zTestPacket = packet2_create(8, P2_TYPE_NORMAL, P2_MODE_NORMAL, 0);
  allocateBuffers();
  initDrawingEnvironment();

  TYRA_LOG("Renderer core initialized!");
}

void RendererCoreGS::initChannels() {
  dma_channel_initialize(DMA_CHANNEL_GIF, nullptr, 0);
}

void RendererCoreGS::allocateBuffers() {
  frameBuffers[0].width = static_cast<unsigned int>(settings->getWidth());
  frameBuffers[0].height = static_cast<unsigned int>(settings->getHeight());
  frameBuffers[0].mask = 0;
  frameBuffers[0].psm = GS_PSM_32;
  frameBuffers[0].address = vram.allocateBuffer(
      frameBuffers[0].width, frameBuffers[0].height, frameBuffers[0].psm);

  frameBuffers[1].width = frameBuffers[0].width;
  frameBuffers[1].height = frameBuffers[0].height;
  frameBuffers[1].mask = frameBuffers[0].mask;
  frameBuffers[1].psm = frameBuffers[0].psm;
  frameBuffers[1].address = vram.allocateBuffer(
      frameBuffers[1].width, frameBuffers[1].height, frameBuffers[1].psm);

  zBuffer.enable = DRAW_ENABLE;
  zBuffer.mask = 0;
  zBuffer.method = ZTEST_METHOD_GREATER_EQUAL;
  zBuffer.zsm = GS_ZBUF_32;
  zBuffer.address = vram.allocateBuffer(frameBuffers[0].width,
                                        frameBuffers[0].height, zBuffer.zsm);

  graph_initialize(frameBuffers[1].address, frameBuffers[1].width,
                   frameBuffers[1].height, frameBuffers[1].psm, 0, 0);

  // Interlacing tests
  // graph_set_mode(GRAPH_MODE_INTERLACED, GRAPH_MODE_NTSC, GRAPH_MODE_FRAME,
  //                GRAPH_ENABLE);
  // graph_set_screen(0, 0, static_cast<int>(settings->getWidth()),
  //                  static_cast<int>(settings->getHeight()));
  // graph_set_bgcolor(0, 0, 0);
  // graph_set_framebuffer_filtered(frameBuffers[1].address,
  // frameBuffers[1].width,
  //                                frameBuffers[1].psm, 0, 0);
  // graph_enable_output();

  TYRA_LOG("Framebuffers, zBuffer set and allocated!");
}

void RendererCoreGS::enableZTests() {
  packet2_reset(zTestPacket, false);
  packet2_update(zTestPacket,
                 draw_enable_tests(zTestPacket->base, 0, &zBuffer));
  packet2_update(zTestPacket, draw_finish(zTestPacket->next));
  dma_channel_wait(DMA_CHANNEL_GIF, 0);
  dma_channel_send_packet2(zTestPacket, DMA_CHANNEL_GIF, true);
}

void RendererCoreGS::initDrawingEnvironment() {
  packet2_t* packet2 = packet2_create(20, P2_TYPE_NORMAL, P2_MODE_NORMAL, 0);
  packet2_update(packet2, draw_setup_environment(packet2->base, 0, frameBuffers,
                                                 &zBuffer));
  packet2_update(packet2, draw_primitive_xyoffset(
                              packet2->next, 0,
                              screenCenter - (settings->getWidth() / 2.0F),
                              screenCenter - (settings->getHeight() / 2.0F)));
  packet2_update(packet2, draw_finish(packet2->next));
  dma_channel_send_packet2(packet2, DMA_CHANNEL_GIF, true);
  dma_channel_wait(DMA_CHANNEL_GIF, 0);
  packet2_free(packet2);
  TYRA_LOG("Drawing environment initialized!");
}

qword_t* RendererCoreGS::setXYOffset(qword_t* q, const int& drawContext,
                                     const float& x, const float& y) {
  PACK_GIFTAG(q, GIF_SET_TAG(1, 0, 0, 0, GIF_FLG_PACKED, 1), GIF_REG_AD);
  q++;

  int yOffset = currentField == GRAPH_FIELD_ODD ? 8 : 0;

  PACK_GIFTAG(q,
              GS_SET_XYOFFSET(static_cast<int>(x * 16.0F),
                              static_cast<int>((y * 16.0F + yOffset))),
              GS_REG_XYOFFSET + drawContext);
  q++;

  return q;
}

void RendererCoreGS::flipBuffers() {
  graph_set_framebuffer_filtered(frameBuffers[context].address,
                                 frameBuffers[context].width,
                                 frameBuffers[context].psm, 0, 0);

  context ^= 1;

  packet2_update(flipPacket,
                 draw_framebuffer(flipPacket->base, 0, &frameBuffers[context]));
  // Interlacing test
  // packet2_update(
  //     flipPacket,
  //     setXYOffset(flipPacket->next, 0,
  //                 screenCenter - (settings->getWidth() / 2.0F),
  //                 screenCenter - (settings->getInterlacedHeightF() / 2.0F)));

  packet2_update(flipPacket, draw_finish(flipPacket->next));
  dma_channel_wait(DMA_CHANNEL_GIF, 0);
  dma_channel_send_packet2(flipPacket, DMA_CHANNEL_GIF, true);
  draw_wait_finish();

  // Interlacing test
  // updateCurrentField();
}

void RendererCoreGS::updateCurrentField() {
  if (*GS_REG_CSR & (1 << 13)) {
    currentField = GRAPH_FIELD_ODD;
    return;
  }

  currentField = GRAPH_FIELD_EVEN;
}

}  // namespace Tyra
