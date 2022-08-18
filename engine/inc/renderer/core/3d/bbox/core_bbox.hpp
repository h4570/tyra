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
#include <string>
#include "./core_bbox_frustum.hpp"
#include "math/m4x4.hpp"
#include "math/plane.hpp"
#include <array>

namespace Tyra {

/** Bounding box */
class CoreBBox {
 public:
  CoreBBox();
  CoreBBox(const CoreBBox& t_bbox);
  explicit CoreBBox(Vec4* t_vertices, u32* faces, u32 t_count);
  explicit CoreBBox(Vec4* t_vertices, u32 t_count);
  explicit CoreBBox(Vec4* t_vertices);
  explicit CoreBBox(CoreBBox** t_bboxes, const u32& count);
  explicit CoreBBox(const std::vector<CoreBBox>& t_bboxes,
                    const u32& startIndex, const u32& stopIndex);
  explicit CoreBBox(const CoreBBox& t_bbox, const M4x4& t_matrix);

  static CoreBBox create(const Vec4& center, const float& size);

  /**
   * 0 - lowX, lowY, lowZ
   * 1 - lowX, lowY, hiZ
   * 2 - lowX, hiY, lowZ
   * 3 - lowX, hiY, hiZ
   * 4 - hiX, lowY, lowZ
   * 5 - hiX, lowY, hiZ
   * 6 - hiX, hiY, lowZ
   * 7 - hiX, hiY, hiZ
   */
  Vec4 vertices[8];

  void operator=(const CoreBBox& v);

  const Vec4& operator[](const u8& i) const { return vertices[i]; }

  const u8 getVertexCount() const { return 8; }

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

  /** Get new transformed BBox by model matrix */
  CoreBBox getTransformed(const M4x4& t_matrix) const;

  /**
   * @brief Check if bbox is in/partially/outside view frustum
   *
   * @param frustumPlanes Available in
   * engine.renderer.core.renderer3D.frustumPlanes
   * @param model Model matrix if you want to fix bbox by model matrix
   * @param margins Optional margins
   */
  CoreBBoxFrustum frustumCheck(const Plane* frustumPlanes, const M4x4& model,
                               const float* margins = nullptr) const;
  CoreBBoxFrustum frustumCheck(const Plane* frustumPlanes,
                               const float* margins = nullptr) const;

  /**
   * @brief Check if bbox is in view frustum
   *
   * @param frustumPlanes Available in
   * engine.renderer.core.renderer3D.frustumPlanes
   * @param model Model matrix if you want to fix bbox by model matrix
   */
  bool isInFrustum(const Plane* frustumPlanes, const M4x4& model) const;
  bool isInFrustum(const Plane* frustumPlanes) const;

 private:
  static std::array<Vec4, 8> frustumCheckVertices;
};

}  // namespace Tyra
