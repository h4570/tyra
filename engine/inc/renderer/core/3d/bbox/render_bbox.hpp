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
#include "./core_bbox.hpp"

namespace Tyra {

/** Bounding box */
class RenderBBox : public CoreBBox {
 public:
  explicit RenderBBox(const Vec4* t_vertices, const u32* faces,
                      const u32& t_count);
  explicit RenderBBox(const Vec4* t_vertices, const u32& t_count);
  explicit RenderBBox(const Vec4* t_vertices);
  explicit RenderBBox(CoreBBox** t_bboxes, const u32& count);
  explicit RenderBBox(const std::vector<CoreBBox>& t_bboxes,
                      const u32& startIndex, const u32& stopIndex);
  explicit RenderBBox(const RenderBBox& t_bbox, const M4x4& t_matrix);

  CoreBBoxFrustum clipFrustumCheck(const Plane* frustumPlanes,
                                   const M4x4& model) const;

  /** Get new transformed BBox by model matrix */
  RenderBBox getTransformed(const M4x4& t_matrix) const;
};

}  // namespace Tyra
