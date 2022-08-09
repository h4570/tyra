/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "../mesh.hpp"
#include "../mesh_frame.hpp"
#include "../mesh_material.hpp"

namespace Tyra {

class StaticMesh : public Mesh {
 public:
  StaticMesh(const MeshBuilder2Data& data);
  StaticMesh(const StaticMesh& mesh);
  ~StaticMesh();

  MeshFrame* getFrame() { return frame; }

  /** @returns bounding box object of current frame. */
  const BBox& getBoundingBox() const { return frame->getBBox(); }

 private:
  MeshFrame* frame;
};

}  // namespace Tyra
