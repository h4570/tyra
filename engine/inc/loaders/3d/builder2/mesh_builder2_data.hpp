
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

#include <vector>
#include "./mesh_builder2_material_data.hpp"

namespace Tyra {

/**
 * Data needed for constructing mesh class.
 * All dynamic data ownership is moved to mesh class, so
 * verts, coords.. are not deallocated!
 */
class MeshBuilder2Data {
 public:
  MeshBuilder2Data();
  ~MeshBuilder2Data();

  std::vector<MeshBuilder2MaterialData*> materials;

  bool textureCoordsEnabled, normalsEnabled, lightMapEnabled;
};

}  // namespace Tyra
