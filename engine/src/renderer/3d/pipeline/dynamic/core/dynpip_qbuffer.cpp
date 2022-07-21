/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/dynamic/core/dynpip_qbuffer.hpp"
#include <sstream>
#include <iomanip>

namespace Tyra {

DynPipQBuffer::DynPipQBuffer() {}

DynPipQBuffer::~DynPipQBuffer() {}

void DynPipQBuffer::setMaxVertCount(const u32& count) { maxVertCount = count; }

void DynPipQBuffer::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void DynPipQBuffer::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string DynPipQBuffer::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "DynPipQBuffer(";
  }
  res << std::fixed << std::setprecision(2);
  res << std::endl;
  res << "Size: " << static_cast<int>(size) << std::endl;

  res << "Vertices from: " << std::endl;
  for (u32 i = 0; i < size; i++)
    res << i << ": " << verticesFrom[i].getPrint() << std::endl;

  res << "Vertices to: " << std::endl;
  for (u32 i = 0; i < size; i++)
    res << i << ": " << verticesTo[i].getPrint() << std::endl;

  if (bag->texture != nullptr) {
    res << "STs from: " << std::endl;
    for (u32 i = 0; i < size; i++)
      res << i << ": " << stsFrom[i].getPrint() << std::endl;

    res << "STs to: " << std::endl;
    for (u32 i = 0; i < size; i++)
      res << i << ": " << stsTo[i].getPrint() << std::endl;
  }

  if (bag->lighting != nullptr) {
    res << "Normals from: " << std::endl;
    for (u32 i = 0; i < size; i++) {
      res << i << ": " << normalsFrom[i].getPrint() << std::endl;
    }

    res << "Normals to: " << std::endl;
    for (u32 i = 0; i < size; i++) {
      res << i << ": " << normalsTo[i].getPrint();
      if (i < size - 1) {
        res << std::endl;
      }
    }
  }

  res << ")";
  return res.str();
}

}  // namespace Tyra
