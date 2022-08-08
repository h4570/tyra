
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "loaders/3d/builder2/mesh_builder2_data.hpp"

namespace Tyra {

MeshBuilder2Data::MeshBuilder2Data() {
  textureCoordsEnabled = false;
  normalsEnabled = false;
  lightMapEnabled = false;
}

MeshBuilder2Data::~MeshBuilder2Data() {
  for (auto& material : materials) {
    delete material;
  }
}

}  // namespace Tyra
