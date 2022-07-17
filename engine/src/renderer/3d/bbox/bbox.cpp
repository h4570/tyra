/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/bbox/bbox.hpp"

namespace Tyra {

BBox::BBox(CoreBBox** t_bboxes, const u32& count) : CoreBBox(t_bboxes, count) {
  setData();
}

BBox::BBox(Vec4* t_vertices, u32 count) : CoreBBox(t_vertices, count) {
  setData();
}

BBox::BBox(Vec4* t_vertices, u32* faces, u32 count)
    : CoreBBox(t_vertices, faces, count) {
  setData();
}

BBox::BBox(Vec4* t_vertices) : CoreBBox(t_vertices) { setData(); }

void BBox::setData() {
  // This might be shortened with Vec4 operator overloading, but current
  // implementation is more human readable.
  _height = _vertices[0].y - _vertices[2].y;
  _width = _vertices[0].x - _vertices[4].x;
  _depth = _vertices[0].z - _vertices[1].z;

  _centerVector = _vertices[0];
  _centerVector.x += (_width / 2);
  _centerVector.y += (_height / 2);
  _centerVector.z += (_depth / 2);

  // Z-Axis faces
  _frontFace = BBoxFace(_vertices[1], _vertices[7], _vertices[1].z);
  _backFace = BBoxFace(_vertices[0], _vertices[6], _vertices[0].z);
  // X-Axis faces
  _leftFace = BBoxFace(_vertices[0], _vertices[3], _vertices[0].x);
  _rightFace = BBoxFace(_vertices[4], _vertices[7], _vertices[4].x);
  // Y-Axis faces
  _topFace = BBoxFace(_vertices[2], _vertices[7], _vertices[2].y);
  _bottomFace = BBoxFace(_vertices[0], _vertices[5], _vertices[0].y);
}

}  // namespace Tyra
