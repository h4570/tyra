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

namespace Tyra {

/** Bounding box */
class CoreBBox {
 public:
  explicit CoreBBox();
  explicit CoreBBox(Vec4* t_vertices, u32* faces, u32 t_count);
  explicit CoreBBox(Vec4* t_vertices, u32 t_count);
  explicit CoreBBox(Vec4* t_vertices);
  explicit CoreBBox(const CoreBBox& t_bbox);
  explicit CoreBBox(CoreBBox** t_bboxes, const u32& count);
  explicit CoreBBox(const std::vector<CoreBBox>& t_bboxes,
                    const u32& startIndex, const u32& stopIndex);

  Vec4 vertices[8];

  const Vec4& operator[](const u8& i) { return vertices[i]; }

  const u8 getVertexCount() { return 8; }

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

  CoreBBoxFrustum isInFrustum(const Plane* frustumPlanes, const M4x4& model,
                              const float* margins = nullptr) const;
};

}  // namespace Tyra
