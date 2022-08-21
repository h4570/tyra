/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "../mesh_frame.hpp"
#include "./dynamic_mesh_animation.hpp"
#include "../mesh.hpp"

namespace Tyra {

class DynamicMesh : public Mesh {
 public:
  explicit DynamicMesh(const MeshBuilderData* data);
  DynamicMesh(const DynamicMesh& mesh);
  ~DynamicMesh();

  std::vector<MeshFrame*> frames;

  DynamicMeshAnimation animation;

  /** @returns bounding box object of current frame. */
  const BBox& getCurrentBoundingBox() const {
    return *frames[animation.getState().currentFrame]->bbox;
  }

  /** Update animation */
  inline void update() { animation.update(); }
};

}  // namespace Tyra
