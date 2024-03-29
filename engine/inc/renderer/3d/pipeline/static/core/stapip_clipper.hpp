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

#include <vector>
#include "debug/debug.hpp"
#include "./stapip_qbuffer.hpp"
#include "renderer/renderer_settings.hpp"
#include "renderer/core/3d/clipper/planes_clip_algorithm.hpp"

namespace Tyra {

/**
 * @brief This class requires VU1 buffer with max buffsize/3 vertices.
 * It will clip triangles and fill the buffer.
 *
 * To be honest clipping algorithm should be moved to VU1 and "AsIs" VU1 program
 * should be renamed to "Clip" - I don't want to do it now, too much time.
 */
class StaPipClipper {
 public:
  StaPipClipper();
  ~StaPipClipper();
  void clip(StaPipQBuffer* buffer);
  void init(const RendererSettings& settings);
  void setMaxVertCount(const u32& count);
  void setMVP(M4x4* mvp);

 private:
  u32 maxVertCount;
  PlanesClipAlgorithm algorithm;
  M4x4* mvp;

  Vec4 inputVerts[3];
  PlanesClipVertexPtrs inputTriangle[3];
  PlanesClipVertex clippedTriangle[9];

  void perspectiveDivide(std::vector<PlanesClipVertex>* vertices);
  void moveDataToBuffer(const std::vector<PlanesClipVertex>& vertices,
                        StaPipQBuffer* buffer);
};

}  // namespace Tyra
