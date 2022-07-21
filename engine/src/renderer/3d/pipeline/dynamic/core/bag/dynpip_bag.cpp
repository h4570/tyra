/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/dynamic/core/bag/dynpip_bag.hpp"
#include <sstream>
#include <iomanip>

namespace Tyra {

DynPipBag::DynPipBag() {
  info = nullptr;
  color = nullptr;
  texture = nullptr;
  lighting = nullptr;
  verticesFrom = nullptr;
  verticesTo = nullptr;
}

DynPipBag::~DynPipBag() {}

void DynPipBag::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void DynPipBag::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string DynPipBag::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "DynPipBag(";
  }
  res << std::fixed << std::setprecision(4);
  res << std::endl;
  res << "Count: " << count << ", " << std::endl;
  res << "Vertices from present: " << (verticesFrom != nullptr ? "Yes" : "No")
      << ", " << std::endl;
  res << "Vertices to present: " << (verticesTo != nullptr ? "Yes" : "No")
      << ", " << std::endl;
  res << "Info present: " << (info != nullptr ? "Yes" : "No") << ", "
      << std::endl;
  res << "Color present: " << (color != nullptr ? "Yes" : "No") << ", "
      << std::endl;
  res << "Texture present: " << (texture != nullptr ? "Yes" : "No") << ", "
      << std::endl;
  res << "Lighting present: " << (lighting != nullptr ? "Yes" : "No") << ", "
      << std::endl;
  res << "Model matrix: " << info->model->getPrint() << ", " << std::endl;
  if (color->single) {
    res << "Color single: " << color->single->getPrint() << ", " << std::endl;
  }
  if (texture) {
    res << "Texture coords from present: "
        << (texture->coordinatesFrom != nullptr ? "Yes" : "No") << ", "
        << std::endl;
    res << "Texture coords to present: "
        << (texture->coordinatesTo != nullptr ? "Yes" : "No") << ", "
        << std::endl;
    res << "Texture: " << texture->texture->getPrint() << ", " << std::endl;
  }

  if (lighting) {
    res << "Lighting normals from present: "
        << (lighting->normalsFrom ? "Yes" : "No") << ", " << std::endl;
    res << "Lighting normals to present: "
        << (lighting->normalsTo ? "Yes" : "No") << ", " << std::endl;
    res << "Lighting matrix: " << lighting->lightMatrix->getPrint() << ", "
        << std::endl;
  }
  res << ")";

  return res.str();
}

}  // namespace Tyra
