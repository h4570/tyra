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
#include <iomanip>
#include "shared/renderer/core/3d/bbox/core_bbox.hpp"

namespace Tyra {

std::array<Vec4, 8> CoreBBox::frustumCheckVertices;

CoreBBox::CoreBBox() {
  for (unsigned int i = 0; i < 8; i++) {
    vertices[i] = Vec4(0.0F, 0.0F, 0.0F, 1.0F);
  }
}

CoreBBox::CoreBBox(CoreBBox** t_bboxes, const unsigned int& count) {
  float lowX = t_bboxes[0]->vertices[0].x;
  float lowY = t_bboxes[0]->vertices[0].y;
  float lowZ = t_bboxes[0]->vertices[0].z;

  float hiX = t_bboxes[0]->vertices[7].x;
  float hiY = t_bboxes[0]->vertices[7].y;
  float hiZ = t_bboxes[0]->vertices[7].z;

  for (unsigned int i = 0; i < count; i++) {
    if (lowX > t_bboxes[i]->vertices[0].x) lowX = t_bboxes[i]->vertices[0].x;
    if (hiX < t_bboxes[i]->vertices[7].x) hiX = t_bboxes[i]->vertices[7].x;

    if (lowY > t_bboxes[i]->vertices[0].y) lowY = t_bboxes[i]->vertices[0].y;
    if (hiY < t_bboxes[i]->vertices[7].y) hiY = t_bboxes[i]->vertices[7].y;

    if (lowZ > t_bboxes[i]->vertices[0].z) lowZ = t_bboxes[i]->vertices[0].z;
    if (hiZ < t_bboxes[i]->vertices[7].z) hiZ = t_bboxes[i]->vertices[7].z;
  }

  vertices[0].set(lowX, lowY, lowZ);
  vertices[1].set(lowX, lowY, hiZ);
  vertices[2].set(lowX, hiY, lowZ);
  vertices[3].set(lowX, hiY, hiZ);

  vertices[4].set(hiX, lowY, lowZ);
  vertices[5].set(hiX, lowY, hiZ);
  vertices[6].set(hiX, hiY, lowZ);
  vertices[7].set(hiX, hiY, hiZ);
}

CoreBBox::CoreBBox(const std::vector<CoreBBox>& t_bboxes,
                   const unsigned int& startIndex,
                   const unsigned int& stopIndex) {
  float lowX = t_bboxes[startIndex].vertices[0].x;
  float lowY = t_bboxes[startIndex].vertices[0].y;
  float lowZ = t_bboxes[startIndex].vertices[0].z;

  float hiX = t_bboxes[startIndex].vertices[7].x;
  float hiY = t_bboxes[startIndex].vertices[7].y;
  float hiZ = t_bboxes[startIndex].vertices[7].z;

  for (unsigned int i = startIndex; i < stopIndex; i++) {
    if (lowX > t_bboxes[i].vertices[0].x) lowX = t_bboxes[i].vertices[0].x;
    if (hiX < t_bboxes[i].vertices[7].x) hiX = t_bboxes[i].vertices[7].x;

    if (lowY > t_bboxes[i].vertices[0].y) lowY = t_bboxes[i].vertices[0].y;
    if (hiY < t_bboxes[i].vertices[7].y) hiY = t_bboxes[i].vertices[7].y;

    if (lowZ > t_bboxes[i].vertices[0].z) lowZ = t_bboxes[i].vertices[0].z;
    if (hiZ < t_bboxes[i].vertices[7].z) hiZ = t_bboxes[i].vertices[7].z;
  }

  vertices[0].set(lowX, lowY, lowZ);
  vertices[1].set(lowX, lowY, hiZ);
  vertices[2].set(lowX, hiY, lowZ);
  vertices[3].set(lowX, hiY, hiZ);

  vertices[4].set(hiX, lowY, lowZ);
  vertices[5].set(hiX, lowY, hiZ);
  vertices[6].set(hiX, hiY, lowZ);
  vertices[7].set(hiX, hiY, hiZ);
}

CoreBBox::CoreBBox(const Vec4* t_vertices, const unsigned int* faces,
                   const unsigned int& count) {
  float lowX, lowY, lowZ, hiX, hiY, hiZ;
  lowX = hiX = t_vertices[faces[0]].x;
  lowY = hiY = t_vertices[faces[0]].y;
  lowZ = hiZ = t_vertices[faces[0]].z;
  for (unsigned int i = 0; i < count; i++) {
    if (lowX > t_vertices[faces[i]].x) lowX = t_vertices[faces[i]].x;
    if (hiX < t_vertices[faces[i]].x) hiX = t_vertices[faces[i]].x;

    if (lowY > t_vertices[faces[i]].y) lowY = t_vertices[faces[i]].y;
    if (hiY < t_vertices[faces[i]].y) hiY = t_vertices[faces[i]].y;

    if (lowZ > t_vertices[faces[i]].z) lowZ = t_vertices[faces[i]].z;
    if (hiZ < t_vertices[faces[i]].z) hiZ = t_vertices[faces[i]].z;
  }

  vertices[0].set(lowX, lowY, lowZ);
  vertices[1].set(lowX, lowY, hiZ);
  vertices[2].set(lowX, hiY, lowZ);
  vertices[3].set(lowX, hiY, hiZ);

  vertices[4].set(hiX, lowY, lowZ);
  vertices[5].set(hiX, lowY, hiZ);
  vertices[6].set(hiX, hiY, lowZ);
  vertices[7].set(hiX, hiY, hiZ);
}

CoreBBox::CoreBBox(const Vec4* t_vertices, const unsigned int& count) {
  float lowX, lowY, lowZ, hiX, hiY, hiZ;
  lowX = hiX = t_vertices[0].x;
  lowY = hiY = t_vertices[0].y;
  lowZ = hiZ = t_vertices[0].z;
  for (unsigned int i = 0; i < count; i++) {
    if (lowX > t_vertices[i].x) lowX = t_vertices[i].x;
    if (hiX < t_vertices[i].x) hiX = t_vertices[i].x;

    if (lowY > t_vertices[i].y) lowY = t_vertices[i].y;
    if (hiY < t_vertices[i].y) hiY = t_vertices[i].y;

    if (lowZ > t_vertices[i].z) lowZ = t_vertices[i].z;
    if (hiZ < t_vertices[i].z) hiZ = t_vertices[i].z;
  }

  vertices[0].set(lowX, lowY, lowZ);
  vertices[1].set(lowX, lowY, hiZ);
  vertices[2].set(lowX, hiY, lowZ);
  vertices[3].set(lowX, hiY, hiZ);

  vertices[4].set(hiX, lowY, lowZ);
  vertices[5].set(hiX, lowY, hiZ);
  vertices[6].set(hiX, hiY, lowZ);
  vertices[7].set(hiX, hiY, hiZ);
}

CoreBBox::CoreBBox(const CoreBBox& t_bbox, const M4x4& t_matrix) {
  for (unsigned int i = 0; i < 8; i++) {
    vertices[i] = t_matrix * t_bbox.vertices[i];
  }
}

CoreBBox::CoreBBox(const CoreBBox& t_bbox) {
  for (auto i = 0; i < 8; i++)
    Vec4::copy(&vertices[i], t_bbox.vertices[i].xyzw);
}

CoreBBox CoreBBox::getTransformed(const M4x4& t_matrix) const {
  return CoreBBox(*this, t_matrix);
}

void CoreBBox::operator=(const CoreBBox& v) {
  for (auto i = 0; i < 8; i++) Vec4::copy(&vertices[i], v.vertices[i].xyzw);
}

CoreBBox::CoreBBox(const Vec4* t_vertices) {
  for (auto i = 0; i < 8; i++) Vec4::copy(&vertices[i], t_vertices[i].xyzw);
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
    res << i << ": " << vertices[i].x << ", " << vertices[i].y << ", "
        << vertices[i].z << ", " << vertices[i].w;

    if (i != 7)
      res << std::endl;
    else
      res << ")";
  }
  return res.str();
}

CoreBBox CoreBBox::create(const Vec4& center, const float& size) {
  CoreBBox bbox;

  float lowX = center.x - size;
  float lowY = center.y - size;
  float lowZ = center.z - size;

  float hiX = center.x + size;
  float hiY = center.y + size;
  float hiZ = center.z + size;

  bbox.vertices[0].set(lowX, lowY, lowZ);
  bbox.vertices[1].set(lowX, lowY, hiZ);
  bbox.vertices[2].set(lowX, hiY, lowZ);
  bbox.vertices[3].set(lowX, hiY, hiZ);

  bbox.vertices[4].set(hiX, lowY, lowZ);
  bbox.vertices[5].set(hiX, lowY, hiZ);
  bbox.vertices[6].set(hiX, hiY, lowZ);
  bbox.vertices[7].set(hiX, hiY, hiZ);

  return bbox;
}

CoreBBoxFrustum CoreBBox::frustumCheck(const Plane* frustumPlanes,
                                       const M4x4& model,
                                       const float* margins) const {
  CoreBBoxFrustum result = IN_FRUSTUM;
  unsigned char boxIn = 0, boxOut = 0;
  char calculatedBboxVertexIndex = -1;

  for (unsigned char i = 0; i < 6; i++) {
    const auto margin = margins == nullptr ? 0.0F : margins[i];
    boxOut = 0;
    boxIn = 0;

    // for each corner of the box do ...
    // get out of the cycle as soon as a box as corners
    // both inside and out of the frustum
    for (unsigned char y = 0; y < 8 && (boxIn == 0 || boxOut == 0); y++) {
      if (y > calculatedBboxVertexIndex) {
        frustumCheckVertices[y] = model * vertices[y];
        calculatedBboxVertexIndex = y;
      }

      auto isOut =
          frustumPlanes[i].distanceTo(frustumCheckVertices[y]) <= margin;

      if (isOut)
        boxOut++;
      else
        boxIn++;
    }

    // if all corners are out
    if (!boxIn)
      return OUTSIDE_FRUSTUM;
    else if (boxOut)
      result = PARTIALLY_IN_FRUSTUM;
  }
  return result;
}

CoreBBoxFrustum CoreBBox::frustumCheck(const Plane* frustumPlanes,
                                       const float* margins) const {
  CoreBBoxFrustum result = IN_FRUSTUM;
  unsigned char boxIn = 0, boxOut = 0;

  for (unsigned char i = 0; i < 6; i++) {
    const auto margin = margins == nullptr ? 0.0F : margins[i];
    boxOut = 0;
    boxIn = 0;

    for (unsigned char y = 0; y < 8 && (boxIn == 0 || boxOut == 0); y++) {
      auto isOut = frustumPlanes[i].distanceTo(vertices[y]) <= margin;

      if (isOut)
        boxOut++;
      else
        boxIn++;
    }

    // if all corners are out
    if (!boxIn)
      return OUTSIDE_FRUSTUM;
    else if (boxOut)
      result = PARTIALLY_IN_FRUSTUM;
  }
  return result;
}

bool CoreBBox::isInFrustum(const Plane* frustumPlanes,
                           const M4x4& model) const {
  char calculatedBboxVertexIndex = -1;

  for (unsigned char i = 0; i < 6; i++) {
    for (unsigned char y = 0; y < 8; y++) {
      if (y > calculatedBboxVertexIndex) {
        frustumCheckVertices[y] = model * vertices[y];
        calculatedBboxVertexIndex = y;
      }

      auto isIn = frustumPlanes[i].distanceTo(frustumCheckVertices[y]) > 0.0F;
      if (isIn) return true;
    }
  }
  return false;
}

bool CoreBBox::isInFrustum(const Plane* frustumPlanes) const {
  for (unsigned char i = 0; i < 6; i++) {
    for (unsigned char y = 0; y < 8; y++) {
      auto isIn = frustumPlanes[i].distanceTo(vertices[y]) > 0.0F;
      if (isIn) return true;
    }
  }

  return false;
}

}  // namespace Tyra
