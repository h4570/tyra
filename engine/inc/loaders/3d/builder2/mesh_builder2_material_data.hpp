
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
#include <string>
#include "./mesh_builder2_material_frame_data.hpp"

namespace Tyra {

class MeshBuilder2MaterialData {
 public:
  MeshBuilder2MaterialData();
  ~MeshBuilder2MaterialData();

  std::vector<MeshBuilder2MaterialFrameData*> frames;
  std::string name;
};

}  // namespace Tyra
