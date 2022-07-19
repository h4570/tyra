/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <string>
#include <sstream>
#include "renderer/core/3d/bbox/core_bbox.hpp"

namespace Tyra {

CoreBBox::CoreBBox(CoreBBox** t_bboxes, const u32& count) {
  float lowX = t_bboxes[0]->_vertices[0].x;
  float lowY = t_bboxes[0]->_vertices[0].y;
  float lowZ = t_bboxes[0]->_vertices[0].z;

  float hiX = t_bboxes[0]->_vertices[7].x;
  float hiY = t_bboxes[0]->_vertices[7].y;
  float hiZ = t_bboxes[0]->_vertices[7].z;

  for (u32 i = 0; i < count; i++) {
    if (lowX > t_bboxes[i]->_vertices[0].x) lowX = t_bboxes[i]->_vertices[0].x;
    if (hiX < t_bboxes[i]->_vertices[7].x) hiX = t_bboxes[i]->_vertices[7].x;

    if (lowY > t_bboxes[i]->_vertices[0].y) lowY = t_bboxes[i]->_vertices[0].y;
    if (hiY < t_bboxes[i]->_vertices[7].y) hiY = t_bboxes[i]->_vertices[7].y;

    if (lowZ > t_bboxes[i]->_vertices[0].z) lowZ = t_bboxes[i]->_vertices[0].z;
    if (hiZ < t_bboxes[i]->_vertices[7].z) hiZ = t_bboxes[i]->_vertices[7].z;
  }

  _vertices[0].set(lowX, lowY, lowZ);
  _vertices[1].set(lowX, lowY, hiZ);
  _vertices[2].set(lowX, hiY, lowZ);
  _vertices[3].set(lowX, hiY, hiZ);

  _vertices[4].set(hiX, lowY, lowZ);
  _vertices[5].set(hiX, lowY, hiZ);
  _vertices[6].set(hiX, hiY, lowZ);
  _vertices[7].set(hiX, hiY, hiZ);
}

CoreBBox::CoreBBox(const std::vector<CoreBBox>& t_bboxes, const u32& startIndex,
                   const u32& stopIndex) {
  float lowX = t_bboxes[startIndex]._vertices[0].x;
  float lowY = t_bboxes[startIndex]._vertices[0].y;
  float lowZ = t_bboxes[startIndex]._vertices[0].z;

  float hiX = t_bboxes[startIndex]._vertices[7].x;
  float hiY = t_bboxes[startIndex]._vertices[7].y;
  float hiZ = t_bboxes[startIndex]._vertices[7].z;

  for (u32 i = startIndex; i < stopIndex; i++) {
    if (lowX > t_bboxes[i]._vertices[0].x) lowX = t_bboxes[i]._vertices[0].x;
    if (hiX < t_bboxes[i]._vertices[7].x) hiX = t_bboxes[i]._vertices[7].x;

    if (lowY > t_bboxes[i]._vertices[0].y) lowY = t_bboxes[i]._vertices[0].y;
    if (hiY < t_bboxes[i]._vertices[7].y) hiY = t_bboxes[i]._vertices[7].y;

    if (lowZ > t_bboxes[i]._vertices[0].z) lowZ = t_bboxes[i]._vertices[0].z;
    if (hiZ < t_bboxes[i]._vertices[7].z) hiZ = t_bboxes[i]._vertices[7].z;
  }

  _vertices[0].set(lowX, lowY, lowZ);
  _vertices[1].set(lowX, lowY, hiZ);
  _vertices[2].set(lowX, hiY, lowZ);
  _vertices[3].set(lowX, hiY, hiZ);

  _vertices[4].set(hiX, lowY, lowZ);
  _vertices[5].set(hiX, lowY, hiZ);
  _vertices[6].set(hiX, hiY, lowZ);
  _vertices[7].set(hiX, hiY, hiZ);
}

CoreBBox::CoreBBox(Vec4* t_vertices, u32* faces, u32 count) {
  float lowX, lowY, lowZ, hiX, hiY, hiZ;
  lowX = hiX = t_vertices[faces[0]].x;
  lowY = hiY = t_vertices[faces[0]].y;
  lowZ = hiZ = t_vertices[faces[0]].z;
  for (u32 i = 0; i < count; i++) {
    if (lowX > t_vertices[faces[i]].x) lowX = t_vertices[faces[i]].x;
    if (hiX < t_vertices[faces[i]].x) hiX = t_vertices[faces[i]].x;

    if (lowY > t_vertices[faces[i]].y) lowY = t_vertices[faces[i]].y;
    if (hiY < t_vertices[faces[i]].y) hiY = t_vertices[faces[i]].y;

    if (lowZ > t_vertices[faces[i]].z) lowZ = t_vertices[faces[i]].z;
    if (hiZ < t_vertices[faces[i]].z) hiZ = t_vertices[faces[i]].z;
  }

  _vertices[0].set(lowX, lowY, lowZ);
  _vertices[1].set(lowX, lowY, hiZ);
  _vertices[2].set(lowX, hiY, lowZ);
  _vertices[3].set(lowX, hiY, hiZ);

  _vertices[4].set(hiX, lowY, lowZ);
  _vertices[5].set(hiX, lowY, hiZ);
  _vertices[6].set(hiX, hiY, lowZ);
  _vertices[7].set(hiX, hiY, hiZ);
}

CoreBBox::CoreBBox(Vec4* t_vertices, u32 count) {
  float lowX, lowY, lowZ, hiX, hiY, hiZ;
  lowX = hiX = t_vertices[0].x;
  lowY = hiY = t_vertices[0].y;
  lowZ = hiZ = t_vertices[0].z;
  for (u32 i = 0; i < count; i++) {
    if (lowX > t_vertices[i].x) lowX = t_vertices[i].x;
    if (hiX < t_vertices[i].x) hiX = t_vertices[i].x;

    if (lowY > t_vertices[i].y) lowY = t_vertices[i].y;
    if (hiY < t_vertices[i].y) hiY = t_vertices[i].y;

    if (lowZ > t_vertices[i].z) lowZ = t_vertices[i].z;
    if (hiZ < t_vertices[i].z) hiZ = t_vertices[i].z;
  }

  _vertices[0].set(lowX, lowY, lowZ);
  _vertices[1].set(lowX, lowY, hiZ);
  _vertices[2].set(lowX, hiY, lowZ);
  _vertices[3].set(lowX, hiY, hiZ);

  _vertices[4].set(hiX, lowY, lowZ);
  _vertices[5].set(hiX, lowY, hiZ);
  _vertices[6].set(hiX, hiY, lowZ);
  _vertices[7].set(hiX, hiY, hiZ);
}

CoreBBox::CoreBBox(Vec4* t_vertices) {
  for (auto i = 0; i < 8; i++) Vec4::copy(&_vertices[i], t_vertices[i].xyzw);
}

void CoreBBox::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void CoreBBox::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string CoreBBox::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "CoreBBox(";
  }
  res << std::fixed << std::setprecision(4);
  res << std::endl;
  for (auto i = 0; i < 8; i++) {
    res << i << ": " << _vertices[i].x << ", " << _vertices[i].y << ", "
        << _vertices[i].z << ", " << _vertices[i].w;

    if (i != 7)
      res << std::endl;
    else
      res << ")";
  }
  return res.str();
}

}  // namespace Tyra
