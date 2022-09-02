
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/renderer/3d/renderer_3d_utility.hpp"
#include "shared/debug/debug.hpp"
#include <math3d.h>
#include <draw.h>
#include <dma.h>
#include <packet2_utils.h>
#include <dma_tags.h>

namespace Tyra {

Renderer3DUtility::Renderer3DUtility() {}

Renderer3DUtility::~Renderer3DUtility() {}

#define TYRA_UTILS_LINE_REGLIST \
  ((u64)GIF_REG_XYZ2) << 0 | ((u64)GIF_REG_XYZ2) << 4

void Renderer3DUtility::init(RendererCore* t_core) {
  core = t_core;

  prim.type = PRIM_LINE;
  prim.shading = PRIM_SHADE_FLAT;
  prim.mapping = DRAW_DISABLE;
  prim.fogging = DRAW_DISABLE;
  prim.blending = DRAW_DISABLE;
  prim.antialiasing = DRAW_ENABLE;
  prim.mapping_type = PRIM_MAP_ST;
  prim.colorfix = PRIM_UNFIXED;

  const auto& settings = core->getSettings();
  planesClipAlgorithm.init(settings);
}

void Renderer3DUtility::drawBox(const Vec4& v, const float& size) {
  drawBox(v, size, Color(192.0F, 32.0F, 32.0F, 128.0F));
}

void Renderer3DUtility::drawLine(const Vec4& from, const Vec4& to) {
  drawLine(from, to, Color(192.0F, 32.0F, 32.0F, 128.0F));
}

void Renderer3DUtility::drawBBox(const CoreBBox& v) {
  drawBBox(v, Color(192.0F, 32.0F, 32.0F, 128.0F));
}

void Renderer3DUtility::drawBox(const Vec4& v, const float& size,
                                const Color& color) {
  auto bbox = CoreBBox::create(v, size);
  drawBBox(bbox, color);
}

void Renderer3DUtility::drawLine(const Vec4& from, const Vec4& to,
                                 const Color& color) {
  const unsigned char vertCount = 2;
  const char thickness = 2;
  short packetSize = 8 * thickness;
  auto gsColor = getGSColor(color);

  auto* packet =
      packet2_create(packetSize, P2_TYPE_NORMAL, P2_MODE_CHAIN, false);

  std::array<Vec4, vertCount> inputVerts;
  inputVerts[0] = from;
  inputVerts[1] = to;

  std::array<xyz_t, vertCount> outputVerts;

  packet2_chain_open_end(packet, 0, 0);
  packet2_update(packet, draw_prim_start(packet->next, 0, &prim, &gsColor));

  for (char i = 0; i < thickness; i++) {
    Vec4 scale(2048.0F + i, 2048.0F + i, static_cast<float>(0xFFFFFF) / 32.0F,
               1.0F);

    auto draw = calcLineVertices(outputVerts.data(), inputVerts[0],
                                 inputVerts[1], scale);

    if (!draw) {
      packet2_free(packet);
      return;
    }

    packet2_add_u64(packet, outputVerts[0].xyz);
    packet2_add_u64(packet, outputVerts[1].xyz);
  }

  packet2_pad128(packet, 0);

  packet2_update(packet,
                 draw_prim_end(packet->next, 2, TYRA_UTILS_LINE_REGLIST));

  packet2_update(packet, draw_finish(packet->next));

  packet2_chain_close_tag(packet);

  dma_channel_wait(DMA_CHANNEL_GIF, 0);
  dma_channel_send_packet2(packet, DMA_CHANNEL_GIF, true);
  dma_channel_wait(DMA_CHANNEL_GIF, 0);

  packet2_free(packet);
}

void Renderer3DUtility::drawBBox(const CoreBBox& v, const Color& color) {
  const unsigned char stripsCount = 6;
  const unsigned char vertCount = 4;
  const char thickness = 2;
  short packetSize = 150 * thickness;
  auto gsColor = getGSColor(color);

  auto* packet =
      packet2_create(packetSize, P2_TYPE_NORMAL, P2_MODE_CHAIN, false);

  std::array<std::array<Vec4, vertCount>, stripsCount> inputVerts;
  fillBBoxVertices(inputVerts, v);

  packet2_chain_open_end(packet, 0, 0);

  bool drawedSomething = false;

  for (char i = 0; i < thickness; i++) {
    Vec4 scale(2048.0F + i, 2048.0F + i, static_cast<float>(0xFFFFFF) / 32.0F,
               1.0F);

    for (char j = 0; j < stripsCount; j++) {
      for (char k = 0; k < vertCount; k++) {
        auto index1 = k;
        auto index2 = (k + 1) % vertCount;

        std::array<xyz_t, 2> outputVerts;
        auto draw = calcLineVertices(outputVerts.data(), inputVerts[j][index1],
                                     inputVerts[j][index2], scale);

        if (!draw) {
          continue;
        }

        drawedSomething = true;

        packet2_update(packet,
                       draw_prim_start(packet->next, 0, &prim, &gsColor));

        packet2_add_u64(packet, outputVerts[0].xyz);
        packet2_add_u64(packet, outputVerts[1].xyz);

        packet2_update(packet,
                       draw_prim_end(packet->next, 2, TYRA_UTILS_LINE_REGLIST));

        packet2_pad128(packet, 0);
      }
    }
  }

  if (!drawedSomething) {
    packet2_free(packet);
    return;
  }

  packet2_update(packet, draw_finish(packet->next));

  packet2_chain_close_tag(packet);

  dma_channel_wait(DMA_CHANNEL_GIF, 0);
  dma_channel_send_packet2(packet, DMA_CHANNEL_GIF, true);
  dma_channel_wait(DMA_CHANNEL_GIF, 0);

  packet2_free(packet);
}

void Renderer3DUtility::fillBBoxVertices(std::array<std::array<Vec4, 4>, 6>& v,
                                         const CoreBBox& bbox) {
  // Up
  /**
   * 2 - lowX, hiY, lowZ
   * 3 - lowX, hiY, hiZ
   * 7 - hiX, hiY, hiZ
   * 6 - hiX, hiY, lowZ
   */
  v[1][0] = bbox.vertices[2];
  v[1][1] = bbox.vertices[3];
  v[1][2] = bbox.vertices[7];
  v[1][3] = bbox.vertices[6];

  // Down
  /**
   * 0 - lowX, lowY, lowZ
   * 1 - lowX, lowY, hiZ
   * 5 - hiX, lowY, hiZ
   * 4 - hiX, lowY, lowZ
   */
  v[0][0] = bbox.vertices[0];
  v[0][1] = bbox.vertices[1];
  v[0][2] = bbox.vertices[5];
  v[0][3] = bbox.vertices[4];

  // Near
  /**
   * 0 - lowX, lowY, lowZ
   * 2 - lowX, hiY, lowZ
   * 6 - hiX, hiY, lowZ
   * 4 - hiX, lowY, lowZ
   */
  v[2][0] = bbox.vertices[0];
  v[2][1] = bbox.vertices[2];
  v[2][2] = bbox.vertices[6];
  v[2][3] = bbox.vertices[4];

  // Far
  /**
   * 1 - lowX, lowY, hiZ
   * 3 - lowX, hiY, hiZ
   * 7 - hiX, hiY, hiZ
   * 5 - hiX, lowY, hiZ
   */
  v[3][0] = bbox.vertices[1];
  v[3][1] = bbox.vertices[3];
  v[3][2] = bbox.vertices[7];
  v[3][3] = bbox.vertices[5];

  // Left
  /**
   * 0 - lowX, lowY, lowZ
   * 1 - lowX, lowY, hiZ
   * 3 - lowX, hiY, hiZ
   * 2 - lowX, hiY, lowZ
   */
  v[4][0] = bbox.vertices[0];
  v[4][1] = bbox.vertices[1];
  v[4][2] = bbox.vertices[3];
  v[4][3] = bbox.vertices[2];

  // Right
  /**
   * 4 - hiX, lowY, lowZ
   * 5 - hiX, lowY, hiZ
   * 7 - hiX, hiY, hiZ
   * 6 - hiX, hiY, lowZ
   */
  v[5][0] = bbox.vertices[4];
  v[5][1] = bbox.vertices[5];
  v[5][2] = bbox.vertices[7];
  v[5][3] = bbox.vertices[6];
}

color_t Renderer3DUtility::getGSColor(const Color& color) {
  color_t gsColor;
  gsColor.r = static_cast<unsigned char>(color.r);
  gsColor.g = static_cast<unsigned char>(color.g);
  gsColor.b = static_cast<unsigned char>(color.b);
  gsColor.a = static_cast<unsigned char>(color.a);
  return gsColor;
}

Vec4 Renderer3DUtility::convertVertices(const Vec4& v, const Vec4& scale) {
  Vec4 output;

  /** Perspective divide + add screen scale */
  asm volatile(
      "lqc2       $vf4, 0x0(%1)         \n\t"
      "lqc2       $vf5, 0x0(%2)         \n\t"
      "vdiv       $Q,   $vf0w,  $vf4w   \n\t"
      "vwaitq                           \n\t"
      "vmulq.xyz  $vf4, $vf4,   $Q      \n\t"
      "vmulaw.xyz $ACC, $vf5,   $vf0w   \n\t"
      "vmadd.xyz  $vf4, $vf4,   $vf5    \n\t"
      "sqc2       $vf4, 0x0(%0)         \n\t"
      :
      : "r"(output.xyzw), "r"(v.xyzw), "r"(scale.xyzw));

  return output;
}

bool Renderer3DUtility::calcLineVertices(xyz_t* outputArray, const Vec4& a,
                                         const Vec4& b, const Vec4& scale) {
  const auto& mvp = core->renderer3D.getViewProj();
  Vec4 tmpA = mvp * a;
  Vec4 tmpB = mvp * b;

  clipInput[0].position = &tmpA;
  clipInput[1].position = &tmpB;
  clipInput[2].position = &tmpA;

  auto clippedSize = planesClipAlgorithm.clip(
      clippedVerts.data(), clipInput.data(), {false, false, false});

  if (clippedSize == 0) {
    return false;
  }

  // To screen space
  auto converted1 = convertVertices(clippedVerts[0].position, scale);
  auto converted2 = convertVertices(clippedVerts[1].position, scale);

  outputArray[0].x = static_cast<unsigned short>(ftoi4(converted1.x));
  outputArray[0].y = static_cast<unsigned short>(ftoi4(converted1.y));
  outputArray[0].z = static_cast<unsigned int>(ftoi4(converted1.z));

  outputArray[1].x = static_cast<unsigned short>(ftoi4(converted2.x));
  outputArray[1].y = static_cast<unsigned short>(ftoi4(converted2.y));
  outputArray[1].z = static_cast<unsigned int>(ftoi4(converted2.z));

  return true;
}

}  // namespace Tyra
