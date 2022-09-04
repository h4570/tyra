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
#include "renderer/3d/pipeline/static/core/bag/packaging/stapip_bag_package.hpp"

namespace Tyra {

StaPipBagPackage::StaPipBagPackage() {
  size = 0;
  bag = nullptr;
  vertices = nullptr;
  sts = nullptr;
  normals = nullptr;
  colors = nullptr;
}
StaPipBagPackage::~StaPipBagPackage() {}

void StaPipBagPackage::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void StaPipBagPackage::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string StaPipBagPackage::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "StaPipBagPackage(";
  }
  res << std::fixed << std::setprecision(2);
  res << std::endl;
  res << "Size: " << static_cast<int>(size) << std::endl;

  res << "Vectors: " << std::endl;
  for (unsigned int i = 0; i < size; i++) {
    res << i << ": " << vertices[i].getPrint() << std::endl;
  }

  if (sts != nullptr) {
    res << "STs: " << std::endl;
    for (unsigned int i = 0; i < size; i++)
      res << i << ": " << sts[i].getPrint() << std::endl;
  }

  if (colors != nullptr) {
    res << "Colors: " << std::endl;
    for (unsigned int i = 0; i < size; i++)
      res << i << ": " << colors[i].getPrint() << std::endl;
  }

  if (normals != nullptr) {
    res << "Normals: " << std::endl;
    for (unsigned int i = 0; i < size; i++)
      res << i << ": " << normals[i].getPrint() << std::endl;
  }

  res << "Is in frustum: ";
  switch (isInFrustum) {
    case CoreBBoxFrustum::IN_FRUSTUM:
      res << "Yes";
      break;
    case CoreBBoxFrustum::OUTSIDE_FRUSTUM:
      res << "No";
      break;
    case CoreBBoxFrustum::PARTIALLY_IN_FRUSTUM:
      res << "Partially";
      break;
  }

  res << ")";
  return res.str();
}

}  // namespace Tyra
