/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <dma.h>
#include <tamtypes.h>
#include <draw.h>
#include <graph.h>
#include <gs_privileged.h>
#include <gs_psm.h>
#include <packet2.h>
#include <packet2_utils.h>
#include <packet2_chain.h>
#include <packet2_types.h>
#include "debug/debug.hpp"
#include "renderer/core/gs/renderer_core_gs.hpp"

namespace Tyra {

RendererCoreGS::RendererCoreGS() {
  context = 0;
  spaceOccupiedByFrameBuffers = 0;
  spaceOccupiedByZBuffer = 0;
}
RendererCoreGS::~RendererCoreGS() {
  if (flipPacket) {
    packet2_free(flipPacket);
  }
}

void RendererCoreGS::init(RendererSettings* t_settings) {
  settings = t_settings;

  initChannels();
  flipPacket = packet2_create(4, P2_TYPE_UNCACHED_ACCL, P2_MODE_NORMAL, 0);
  allocateBuffers();
  initDrawingEnvironment();
  setPrim();
  setLod();

  TYRA_LOG("Renderer core initialized!");
}

void RendererCoreGS::initChannels() {
  dma_channel_initialize(DMA_CHANNEL_GIF, NULL, 0);
}

void RendererCoreGS::allocateBuffers() {
  const u16 psm = 32;

  frameBuffers[0].width = static_cast<unsigned int>(settings->getWidth());
  frameBuffers[0].height = settings->getInterlacedHeightUI();
  frameBuffers[0].mask = 0;
  frameBuffers[0].psm = GS_PSM_32;
  frameBuffers[0].address =
      graph_vram_allocate(frameBuffers[0].width, frameBuffers[0].height,
                          frameBuffers[0].psm, GRAPH_ALIGN_PAGE);

  frameBuffers[1].width = frameBuffers[0].width;
  frameBuffers[1].height = frameBuffers[0].height;
  frameBuffers[1].mask = frameBuffers[0].mask;
  frameBuffers[1].psm = frameBuffers[0].psm;
  frameBuffers[1].address =
      graph_vram_allocate(frameBuffers[1].width, frameBuffers[1].height,
                          frameBuffers[1].psm, GRAPH_ALIGN_PAGE);

  zBuffer.enable = DRAW_ENABLE;
  zBuffer.mask = 0;
  zBuffer.method = ZTEST_METHOD_GREATER_EQUAL;
  zBuffer.zsm = GS_ZBUF_32;
  zBuffer.address =
      graph_vram_allocate(frameBuffers[0].width, frameBuffers[0].height,
                          zBuffer.zsm, GRAPH_ALIGN_PAGE);
  TYRA_LOG("Framebuffers, zBuffer set and allocated!");

  graph_set_mode(GRAPH_MODE_INTERLACED, GRAPH_MODE_NTSC, GRAPH_MODE_FRAME,
                 GRAPH_ENABLE);
  graph_set_screen(0, 0, static_cast<int>(settings->getWidth()),
                   static_cast<int>(settings->getHeight()));
  graph_set_bgcolor(0, 0, 0);
  graph_set_framebuffer_filtered(frameBuffers[1].address, frameBuffers[1].width,
                                 frameBuffers[1].psm, 0, 0);
  graph_enable_output();

  spaceOccupiedByFrameBuffers =
      ((frameBuffers[0].width / 100.0F) * (frameBuffers[0].height / 100.0F) *
       (psm / 100.0F)) /
      8.0F;

  spaceOccupiedByZBuffer = spaceOccupiedByFrameBuffers;
  spaceOccupiedByFrameBuffers *= 2;
}

void RendererCoreGS::initDrawingEnvironment() {
  packet2_t* packet2 = packet2_create(20, P2_TYPE_NORMAL, P2_MODE_NORMAL, 0);
  packet2_update(packet2, draw_setup_environment(packet2->base, 0, frameBuffers,
                                                 &zBuffer));
  packet2_update(
      packet2,
      draw_primitive_xyoffset(
          packet2->next, 0, screenCenter - (settings->getWidth() / 2.0F),
          screenCenter - (settings->getInterlacedHeightF() / 2.0F)));
  packet2_update(packet2, draw_finish(packet2->next));
  dma_channel_send_packet2(packet2, DMA_CHANNEL_GIF, true);
  dma_channel_wait(DMA_CHANNEL_GIF, 0);
  packet2_free(packet2);
  TYRA_LOG("Drawing environment initialized!");
}

void RendererCoreGS::setPrim() {
  prim.type = PRIM_TRIANGLE;
  prim.shading = PRIM_SHADE_GOURAUD;
  prim.mapping = DRAW_ENABLE;
  prim.fogging = DRAW_DISABLE;
  prim.blending = DRAW_ENABLE;
  prim.antialiasing = DRAW_DISABLE;
  prim.mapping_type = PRIM_MAP_ST;
  prim.colorfix = PRIM_UNFIXED;
  TYRA_LOG("Prim set!");
}

void RendererCoreGS::setLod() {
  lod.calculation = LOD_USE_K;
  lod.max_level = 0;
  lod.mag_filter = LOD_MAG_NEAREST;
  lod.min_filter = LOD_MIN_NEAREST;
  lod.l = 0;
  lod.k = 0.0F;
  TYRA_LOG("Lod set!");
}

void RendererCoreGS::flipBuffers() {
  graph_set_framebuffer_filtered(frameBuffers[context].address,
                                 frameBuffers[context].width,
                                 frameBuffers[context].psm, 0, 0);

  context ^= 1;

  packet2_update(flipPacket,
                 draw_framebuffer(flipPacket->base, 0, &frameBuffers[context]));
  packet2_update(flipPacket, draw_finish(flipPacket->next));
  dma_channel_wait(DMA_CHANNEL_GIF, 0);
  dma_channel_send_packet2(flipPacket, DMA_CHANNEL_GIF, true);
  draw_wait_finish();
}

}  // namespace Tyra
