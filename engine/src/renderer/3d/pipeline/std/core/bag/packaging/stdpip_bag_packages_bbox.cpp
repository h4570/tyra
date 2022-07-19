/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tamtypes.h>
#include <string>
#include <sstream>
#include <vector>
#include "debug/debug.hpp"
#include "renderer/3d/pipeline/std/core/bag/packaging/stdpip_bag_packages_bbox.hpp"

namespace Tyra {

StdpipBagPackagesBBox::StdpipBagPackagesBBox(Vec4* t_vertices, u32* t_faces,
                                             const u32& t_facesCount,
                                             const u32& t_maxVertCount) {
  u32 splitPartSize = t_maxVertCount / 3;
  vertexCount = t_facesCount;
  partsCount = ceil(vertexCount / static_cast<float>(splitPartSize));

  bboxParts = new std::vector<CoreBBox>;
  for (u32 i = 0; i < partsCount; i++) {
    u32 partSize =
        i == partsCount - 1 ? t_facesCount - i * splitPartSize : splitPartSize;
    bboxParts->push_back(
        CoreBBox(t_vertices, t_faces + i * splitPartSize, partSize));
  }
  mainBBox = new RenderBBox(*bboxParts, 0, partsCount);
}

StdpipBagPackagesBBox::StdpipBagPackagesBBox(Vec4* t_vertices,
                                             const u32& t_count,
                                             const u32& t_maxVertCount) {
  u32 splitPartSize = t_maxVertCount / 3;
  vertexCount = t_count;
  partsCount = ceil(vertexCount / static_cast<float>(splitPartSize));

  bboxParts = new std::vector<CoreBBox>;
  for (u32 i = 0; i < partsCount; i++) {
    u32 partSize =
        i == partsCount - 1 ? t_count - i * splitPartSize : splitPartSize;
    bboxParts->push_back(RenderBBox(t_vertices + i * splitPartSize, partSize));
  }

  mainBBox = new RenderBBox(*bboxParts, 0, partsCount);
}

const RenderBBox& StdpipBagPackagesBBox::getChildBBox1By3(
    const u32& index) const {
  TYRA_ASSERT(index < partsCount,
              "Index out of range. Provided index: ", index);
  return static_cast<RenderBBox&>(bboxParts->at(index));
}

RenderBBox* StdpipBagPackagesBBox::getMainBBox() { return mainBBox; }

const u32& StdpipBagPackagesBBox::getPartsCount() const { return partsCount; }

const u32& StdpipBagPackagesBBox::getVertexCount() const { return vertexCount; }

RenderBBox StdpipBagPackagesBBox::createChildBBox(const u32& index,
                                                  const u16& partsSize) const {
  return RenderBBox(*bboxParts, index, partsSize);
}

void StdpipBagPackagesBBox::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void StdpipBagPackagesBBox::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string StdpipBagPackagesBBox::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "StdpipBagPackagesBBox(";
  }
  res << std::fixed << std::setprecision(2);
  res << std::endl;
  res << "Vertices count: " << static_cast<int>(vertexCount) << std::endl;

  res << "Main CoreBBox: " << std::endl;
  res << mainBBox->getPrint() << std::endl;

  res << "Child BBoxes: " << std::endl;
  for (u32 i = 0; i < partsCount; i++) {
    res << i << ": " << bboxParts->at(i).getPrint();
    if (i != partsCount - 1) {
      res << std::endl;
    }
  }

  res << ")";
  return res.str();
}

StdpipBagPackagesBBox::~StdpipBagPackagesBBox() {
  delete bboxParts;
  delete mainBBox;
}

}  // namespace Tyra
