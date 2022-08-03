
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "loaders/3d/builder2/mesh_builder2_material_frame_data.hpp"

namespace Tyra {

MeshBuilder2MaterialFrameData::MeshBuilder2MaterialFrameData() {
  vertices = nullptr;
  textureCoords = nullptr;
  normals = nullptr;
  colors = nullptr;
}
MeshBuilder2MaterialFrameData::~MeshBuilder2MaterialFrameData() {}

}  // namespace Tyra
