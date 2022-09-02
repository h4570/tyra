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
#include <vector>
#include "debug/debug.hpp"
#include "renderer/3d/pipeline/static/core/bag/packaging/stapip_bag_packages_bbox.hpp"

namespace Tyra {

StaPipBagPackagesBBox::StaPipBagPackagesBBox(
    const Vec4* t_vertices, unsigned int* t_faces,
    const unsigned int& t_facesCount, const unsigned int& t_maxVertCount) {
  unsigned int splitPartSize = t_maxVertCount / 3;
  vertexCount = t_facesCount;
  partsCount = ceil(vertexCount / static_cast<float>(splitPartSize));

  bboxParts = new std::vector<CoreBBox>;
  for (unsigned int i = 0; i < partsCount; i++) {
    unsigned int partSize =
        i == partsCount - 1 ? t_facesCount - i * splitPartSize : splitPartSize;
    bboxParts->push_back(
        CoreBBox(t_vertices, t_faces + i * splitPartSize, partSize));
  }
  mainBBox = new RenderBBox(*bboxParts, 0, partsCount);
}

StaPipBagPackagesBBox::StaPipBagPackagesBBox(
    const Vec4* t_vertices, const unsigned int& t_count,
    const unsigned int& t_maxVertCount) {
  unsigned int splitPartSize = t_maxVertCount / 3;
  vertexCount = t_count;
  partsCount = ceil(vertexCount / static_cast<float>(splitPartSize));

  bboxParts = new std::vector<CoreBBox>;
  for (unsigned int i = 0; i < partsCount; i++) {
    unsigned int partSize =
        i == partsCount - 1 ? t_count - i * splitPartSize : splitPartSize;
    bboxParts->push_back(RenderBBox(t_vertices + i * splitPartSize, partSize));
  }

  mainBBox = new RenderBBox(*bboxParts, 0, partsCount);
}

StaPipBagPackagesBBox::~StaPipBagPackagesBBox() {
  delete bboxParts;
  delete mainBBox;
}

const RenderBBox& StaPipBagPackagesBBox::getChildBBox1By3(
    const unsigned int& index) const {
  TYRA_ASSERT(index < partsCount,
              "Index out of range. Provided index: ", index);
  return static_cast<RenderBBox&>(bboxParts->at(index));
}

RenderBBox* StaPipBagPackagesBBox::getMainBBox() { return mainBBox; }

const unsigned int& StaPipBagPackagesBBox::getPartsCount() const {
  return partsCount;
}

const unsigned int& StaPipBagPackagesBBox::getVertexCount() const {
  return vertexCount;
}

RenderBBox StaPipBagPackagesBBox::createChildBBox(
    const unsigned int& index, const unsigned short& partsSize) const {
  return RenderBBox(*bboxParts, index, index + partsSize);
}

void StaPipBagPackagesBBox::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void StaPipBagPackagesBBox::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string StaPipBagPackagesBBox::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "StaPipBagPackagesBBox(";
  }
  res << std::fixed << std::setprecision(2);
  res << std::endl;
  res << "Vertices count: " << static_cast<int>(vertexCount) << std::endl;

  res << "Main CoreBBox: " << std::endl;
  res << mainBBox->getPrint() << std::endl;

  res << "Child BBoxes: " << std::endl;
  for (unsigned int i = 0; i < partsCount; i++) {
    res << i << ": " << bboxParts->at(i).getPrint();
    if (i != partsCount - 1) {
      res << std::endl;
    }
  }

  res << ")";
  return res.str();
}

}  // namespace Tyra
