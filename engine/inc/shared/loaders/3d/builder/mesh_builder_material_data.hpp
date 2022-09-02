
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

#include <vector>
#include <string>
#include <optional>
#include "./mesh_builder_material_frame_data.hpp"
#include "shared/renderer/models/color.hpp"

namespace Tyra {

class MeshBuilderMaterialData {
 public:
  MeshBuilderMaterialData();
  ~MeshBuilderMaterialData();

  std::vector<MeshBuilderMaterialFrameData*> frames;
  std::string name;
  std::optional<std::string> texturePath;
  Color ambient;
};

}  // namespace Tyra
