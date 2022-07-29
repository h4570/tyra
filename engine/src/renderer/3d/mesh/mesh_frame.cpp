
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "debug/debug.hpp"
#include <tamtypes.h>
#include <iomanip>
#include <string>
#include "math/vec4.hpp"
#include "renderer/models/color.hpp"
#include "renderer/3d/mesh/mesh_frame.hpp"

namespace Tyra {

MeshFrame::MeshFrame(const MeshBuilderData& data, const u32& index) {
  TYRA_ASSERT(index < data.framesCount && index >= 0, "Provided index \"",
              index, "\" is out of range");

  id = rand() % 1000000;

  bbox =
      new BBox(data.frames[index]->vertices, data.frames[index]->verticesCount);

  _isMother = true;
}

MeshFrame::MeshFrame(const MeshFrame& frame) {
  id = rand() % 1000000;

  bbox = frame.bbox;

  _isMother = false;
}

MeshFrame::~MeshFrame() {
  if (_isMother) {
    delete bbox;
  }
}

void MeshFrame::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void MeshFrame::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string MeshFrame::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "MeshFrame(";
  }
  res << std::fixed << std::setprecision(2);

  res << "Id: " << id << ", " << std::endl;
  res << "BBox: " << bbox->getPrint() << ", " << std::endl;
  res << ")";

  return res.str();
}

}  // namespace Tyra
