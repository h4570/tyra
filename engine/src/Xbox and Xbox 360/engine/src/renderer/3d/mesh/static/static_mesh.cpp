
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/mesh/static/static_mesh.hpp"

namespace Tyra {

StaticMesh::StaticMesh(const MeshBuilderData* data) : Mesh(data) {
  if (data->materials[0]->frames.size() > 1)
    TYRA_WARN("Static meshes should have only one frame, but ",
              data->materials[0]->frames.size(), " frames were found");

  frame = new MeshFrame(*data, 0);
}

StaticMesh::StaticMesh(const StaticMesh& mesh) : Mesh(mesh) {
  frame = new MeshFrame(*mesh.frame);
}

StaticMesh::~StaticMesh() { delete frame; }

}  // namespace Tyra
