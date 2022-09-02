/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/renderer/3d/bbox/bbox.hpp"

namespace Tyra {

BBox::BBox(CoreBBox** t_bboxes, const unsigned int& count)
    : CoreBBox(t_bboxes, count) {
  setData();
}

BBox::BBox(const Vec4* t_vertices, const unsigned int& count)
    : CoreBBox(t_vertices, count) {
  setData();
}

BBox::BBox(const Vec4* t_vertices, const unsigned int* faces,
           const unsigned int& count)
    : CoreBBox(t_vertices, faces, count) {
  setData();
}

BBox::BBox(const BBox& t_bbox) : CoreBBox(t_bbox) { setData(); }

BBox::BBox(const BBox& t_bbox, const M4x4& t_matrix)
    : CoreBBox(t_bbox, t_matrix) {
  setData();
}

BBox::BBox(const Vec4* t_vertices) : CoreBBox(t_vertices) { setData(); }

BBox BBox::getTransformed(const M4x4& t_matrix) const {
  return BBox(*this, t_matrix);
}

void BBox::setData() {
  // This might be shortened with Vec4 operator overloading, but current
  // implementation is more human readable.
  _height = vertices[0].y - vertices[2].y;
  _width = vertices[0].x - vertices[4].x;
  _depth = vertices[0].z - vertices[1].z;

  _centerVector = vertices[0];
  _centerVector.x += (_width / 2);
  _centerVector.y += (_height / 2);
  _centerVector.z += (_depth / 2);

  // Z-Axis faces
  _frontFace = BBoxFace(vertices[1], vertices[7], vertices[1].z);
  _backFace = BBoxFace(vertices[0], vertices[6], vertices[0].z);
  // X-Axis faces
  _leftFace = BBoxFace(vertices[0], vertices[3], vertices[0].x);
  _rightFace = BBoxFace(vertices[4], vertices[7], vertices[4].x);
  // Y-Axis faces
  _topFace = BBoxFace(vertices[2], vertices[7], vertices[2].y);
  _bottomFace = BBoxFace(vertices[0], vertices[5], vertices[0].y);
}

Vec4 BBox::min() const {
  Vec4 temp, _min;
  unsigned char isInitialized = 0;

  for (unsigned char i = 0; i < 8; i++) {
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

Vec4 BBox::max() const {
  Vec4 temp, _max;
  unsigned char isInitialized = 0;

  for (unsigned char i = 0; i < 8; i++) {
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

BBox BBox::create(const Vec4& center, const float& size) {
  auto core = CoreBBox::create(center, size);
  return BBox(core.vertices);
}

void BBox::operator=(const BBox& v) {
  for (auto i = 0; i < 8; i++) Vec4::copy(&vertices[i], v.vertices[i].xyzw);
  setData();
}

void BBox::getMinMax(Vec4* res_min, Vec4* res_max) const {
  Vec4 temp = Vec4();

  unsigned char isInitialized = 0;
  for (unsigned char i = 0; i < 8; i++) {
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
