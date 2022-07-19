/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <vector>
#include "./core_bbox_frustum.hpp"
#include "math/m4x4.hpp"
#include "math/plane.hpp"

namespace Tyra {

/** Bounding box */
class CoreBBox {
 public:
  explicit CoreBBox(Vec4* t_vertices, u32* faces, u32 t_count);
  explicit CoreBBox(Vec4* t_vertices, u32 t_count);
  explicit CoreBBox(Vec4* t_vertices);
  explicit CoreBBox(CoreBBox** t_bboxes, const u32& count);
  explicit CoreBBox(const std::vector<CoreBBox>& t_bboxes,
                    const u32& startIndex, const u32& stopIndex);

  const Vec4& operator[](const u8& i) { return _vertices[i]; }
  const Vec4& getVertex(const u8& i) { return _vertices[i]; }
  Vec4* getVertices() { return _vertices; }

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 protected:
  Vec4 _vertices[8];
};

}  // namespace Tyra
