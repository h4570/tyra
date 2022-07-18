
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/mesh/static/static_mesh.hpp"

namespace Tyra {

StaticMesh::StaticMesh(const MeshBuilderData& data) : Mesh(data) {
  TYRA_ASSERT(data.framesCount == 1, "Static meshes must have only one frame");
  frame = new MeshFrame(data, 0);
}

StaticMesh::StaticMesh(const StaticMesh& mesh) : Mesh(mesh) {
  frame = new MeshFrame(*mesh.frame);
}

StaticMesh::~StaticMesh() { delete frame; }

}  // namespace Tyra
