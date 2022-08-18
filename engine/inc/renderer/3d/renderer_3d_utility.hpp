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

#include "math/vec4.hpp"
#include "renderer/models/color.hpp"
#include "renderer/core/3d/bbox/core_bbox.hpp"
#include "../core/renderer_core.hpp"
#include "renderer/core/3d/clipper/planes_clip_algorithm.hpp"

namespace Tyra {

/**
 * Debug functions, without z buffer, with bad performance.
 *
 * Remember to use these functions AFTER all 3D rendering,
 * otherwise lines will be overwritten! There is Github issue about that.
 */
class Renderer3DUtility {
 public:
  Renderer3DUtility();
  ~Renderer3DUtility();

  void init(RendererCore* core);

  void drawBox(const Vec4& v, const float& size);
  void drawBox(const Vec4& v, const float& size, const Color& color);

  void drawLine(const Vec4& from, const Vec4& to);
  void drawLine(const Vec4& from, const Vec4& to, const Color& color);

  void drawBBox(const CoreBBox& v);
  void drawBBox(const CoreBBox& v, const Color& color);

 private:
  std::array<PlanesClipVertex, 9> clippedVerts;
  std::array<PlanesClipVertexPtrs, 3> clipInput;

  RendererCore* core;
  prim_t prim;

  Vec4 convertVertices(const Vec4& v, const Vec4& scale);

  void fillBBoxVertices(std::array<std::array<Vec4, 4>, 6>& v,
                        const CoreBBox& bbox);

  color_t getGSColor(const Color& color);

  /**
   * @return true - To draw
   * @return false - To skip (outside view frustum)
   */
  bool calcLineVertices(xyz_t* outputArray, const Vec4& a, const Vec4& b,
                        const Vec4& scale);

  PlanesClipAlgorithm planesClipAlgorithm;
};

}  // namespace Tyra
