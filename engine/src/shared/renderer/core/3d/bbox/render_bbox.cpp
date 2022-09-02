/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <string>
#include <sstream>
#include "shared/renderer/core/3d/bbox/render_bbox.hpp"

namespace Tyra {

RenderBBox::RenderBBox(CoreBBox** t_bboxes, const unsigned int& count)
    : CoreBBox(t_bboxes, count) {}

RenderBBox::RenderBBox(const std::vector<CoreBBox>& t_bboxes,
                       const unsigned int& startIndex,
                       const unsigned int& stopIndex)
    : CoreBBox(t_bboxes, startIndex, stopIndex) {}

RenderBBox::RenderBBox(const Vec4* t_vertices, const unsigned int* faces,
                       const unsigned int& count)
    : CoreBBox(t_vertices, faces, count) {}

RenderBBox::RenderBBox(const Vec4* t_vertices, const unsigned int& count)
    : CoreBBox(t_vertices, count) {}

RenderBBox::RenderBBox(const Vec4* t_vertices) : CoreBBox(t_vertices) {}

RenderBBox::RenderBBox(const RenderBBox& t_bbox, const M4x4& t_matrix)
    : CoreBBox(t_bbox, t_matrix) {}

RenderBBox RenderBBox::getTransformed(const M4x4& t_matrix) const {
  return RenderBBox(*this, t_matrix);
}

/**
 * @brief Frustum checker for renderer.
 * Background: We want to really put as low as possible polys to clipper.
 * So we are doing magic trick. If BBox is partially inside frustum (clipper),
 * we are adding some margins, and checking again if it really needs clipping,
 * because "Cull" renderer can handle easy clip cases and its faster.
 */
CoreBBoxFrustum RenderBBox::clipFrustumCheck(const Plane* frustumPlanes,
                                             const M4x4& model) const {
  auto result = frustumCheck(frustumPlanes, model);

  if (result != PARTIALLY_IN_FRUSTUM) {
    return result;
  }

  // Oh no, it probably needs clipping

  // This is crappy guard band, but it works xd
  float guardBand[6];  // This probably needs more calibration

  guardBand[0] = -15.0F;  // Top
  guardBand[1] = -10.0F;  // BOTTOM
  guardBand[2] = -25.0F;  // LEFT
  guardBand[3] = -25.0F;  // RIGHT
  guardBand[4] = -10.0F;  // NEAR
  guardBand[5] = -10.0F;  // FAR

  return frustumCheck(frustumPlanes, model, guardBand);  // Let's check it again
}

}  // namespace Tyra
