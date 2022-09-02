
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/loaders/3d/builder/mesh_builder_data.hpp"

namespace Tyra {

MeshBuilderData::MeshBuilderData() {
  loadNormals = false;
  loadLightmap = false;
}

MeshBuilderData::~MeshBuilderData() {
  for (auto& material : materials) {
    delete material;
  }
}

}  // namespace Tyra
