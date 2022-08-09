
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

MeshFrame::MeshFrame(const MeshBuilder2Data& data, const u32& index) {
  id = rand() % 1000000;

  auto bboxes = new CoreBBox*[data.materials.size()];
  for (u32 i = 0; i < data.materials.size(); i++) {
    bboxes[i] = new CoreBBox(data.materials[i]->frames[index]->vertices,
                             data.materials[i]->frames[index]->count);
  }

  bbox = new BBox(bboxes, data.materials.size());

  for (u32 i = 0; i < data.materials.size(); i++) delete bboxes[i];
  delete[] bboxes;

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
