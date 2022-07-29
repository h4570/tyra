/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
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

BBox::BBox(const BBox& t_bbox) : CoreBBox(t_bbox) { setData(); }

BBox::BBox(const BBox& t_bbox, const M4x4& t_matrix) {
  for (u32 i = 0; i < 8; i++) {
    _vertices[i] = t_matrix * _vertices[i];
  }

  setData();
}

BBox::BBox(Vec4* t_vertices) : CoreBBox(t_vertices) { setData(); }

BBox BBox::getTransformed(const M4x4& t_matrix) {
  return BBox(*this, t_matrix);
}

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

Vec4 BBox::min() {
  Vec4 temp, _min;
  u8 isInitialized = 0;

  const Vec4* vertices = getVertices();
  for (u8 i = 0; i < 8; i++) {
    temp.set(vertices[i].x, vertices[i].y, vertices[i].z, 1.0F);
    if (isInitialized == 0) {
      isInitialized = 1;
      _min.set(temp);
    }

    if (_min.x > temp.x) _min.x = temp.x;
    if (_min.y > temp.y) _min.y = temp.y;
    if (_min.z > temp.z) _min.z = temp.z;
  }

  return _min;
}

Vec4 BBox::max() {
  Vec4 temp, _max;
  u8 isInitialized = 0;

  const Vec4* vertices = getVertices();
  for (u8 i = 0; i < 8; i++) {
    temp.set(vertices[i].x, vertices[i].y, vertices[i].z, 1.0F);
    if (isInitialized == 0) {
      isInitialized = 1;
      _max.set(temp);
    }

    if (temp.x > _max.x) _max.x = temp.x;
    if (temp.y > _max.y) _max.y = temp.y;
    if (temp.z > _max.z) _max.z = temp.z;
  }

  return _max;
}

void BBox::getMinMax(Vec4* res_min, Vec4* res_max) {
  Vec4 temp = Vec4();

  u8 isInitialized = 0;
  const Vec4* vertices = getVertices();
  for (u8 i = 0; i < 8; i++) {
    temp.set(vertices[i].x, vertices[i].y, vertices[i].z, 1.0F);
    if (isInitialized == 0) {
      isInitialized = 1;
      res_min->set(temp);
      res_max->set(temp);
    }

    if (res_min->x > temp.x) res_min->x = temp.x;
    if (temp.x > res_max->x) res_max->x = temp.x;

    if (res_min->y > temp.y) res_min->y = temp.y;
    if (temp.y > res_max->y) res_max->y = temp.y;

    if (res_min->z > temp.z) res_min->z = temp.z;
    if (temp.z > res_max->z) res_max->z = temp.z;
  }
}

}  // namespace Tyra
