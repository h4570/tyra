
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/renderer_3d_utility.hpp"
#include "debug/debug.hpp"
#include <math3d.h>
#include <draw.h>
#include <dma.h>
#include <packet2_utils.h>
#include <dma_tags.h>

namespace Tyra {

Renderer3DUtility::Renderer3DUtility() {}

Renderer3DUtility::~Renderer3DUtility() {}

void Renderer3DUtility::init(RendererCore* t_core) {
  core = t_core;

  basePrim.type = PRIM_LINE;
  basePrim.shading = PRIM_SHADE_FLAT;
  basePrim.mapping = DRAW_DISABLE;
  basePrim.fogging = DRAW_DISABLE;
  basePrim.blending = DRAW_DISABLE;
  basePrim.antialiasing = DRAW_DISABLE;
  basePrim.mapping_type = PRIM_MAP_ST;
  basePrim.colorfix = PRIM_UNFIXED;

  baseColor.r = 0x80;
  baseColor.g = 0x80;
  baseColor.b = 0x80;
  baseColor.a = 0x80;
  baseColor.q = 1.0F;
}

void Renderer3DUtility::drawPoint(const Vec4& v) {
  drawPoint(v, Color(128.0F, 0.0F, 0.0F, 128.0F));
}

void Renderer3DUtility::drawLine(const Vec4& from, const Vec4& to) {
  drawLine(from, to, Color(255.0F, 32.0F, 32.0F, 128.0F));
}

void Renderer3DUtility::drawBBox(const CoreBBox& v) {
  drawBBox(v, Color(255.0F, 32.0F, 32.0F, 128.0F));
}

void Renderer3DUtility::drawPoint(const Vec4& v, const Color& color) {
  // TODO
}

void Renderer3DUtility::drawLine(const Vec4& from, const Vec4& to,
                                 const Color& color) {
  const u16 packetSize = 20;
  auto* packet = packet2_create(20, P2_TYPE_NORMAL, P2_MODE_CHAIN, false);

  auto mvp = core->renderer3D.getViewProj();
  auto* inputVerts = new Vec4[2];
  auto* inputColors = new color_f_t[2];
  inputVerts[0] = mvp * from;
  inputVerts[1] = mvp * to;
  inputColors[0] = {color.r, color.g, color.b, color.a};
  inputColors[1] = {color.r, color.g, color.b, color.a};

  auto* outputVerts = new xyz_t[2];
  auto* outputColors = new color_t[2];
  draw_convert_xyz(outputVerts, 2048, 2048, static_cast<int>(0xFFFFFF / 32.0F),
                   2, reinterpret_cast<vertex_f_t*>(inputVerts));
  draw_convert_rgbq(outputColors, 2, reinterpret_cast<vertex_f_t*>(inputVerts),
                    inputColors, 0x80);

  packet2_chain_open_end(packet, 0, 0);

  basePrim.type = PRIM_LINE;

  packet2_update(packet,
                 draw_prim_start(packet->next, 0, &basePrim, &baseColor));
  for (auto i = 0; i < 2; i++) {
    packet2_add_u64(packet, outputColors[i].rgbaq);
    packet2_add_u64(packet, outputVerts[i].xyz);
  }
  packet2_update(packet, draw_prim_end(packet->next, 2, DRAW_RGBAQ_REGLIST));

  packet2_update(packet, draw_finish(packet->next));

  packet2_chain_close_tag(packet);

  TYRA_ASSERT(packet2_get_qw_count(packet) < packetSize);

  dma_channel_wait(DMA_CHANNEL_GIF, 0);
  dma_channel_send_packet2(packet, DMA_CHANNEL_GIF, true);
  dma_channel_wait(DMA_CHANNEL_GIF, 0);

  delete[] inputVerts;
  delete[] inputColors;
  delete[] outputVerts;
  delete[] outputColors;
}

void Renderer3DUtility::drawBBox(const CoreBBox& v, const Color& color) {
  // TODO
}

}  // namespace Tyra
