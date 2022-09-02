
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "loaders/3d/builder/mesh_builder_material_data.hpp"

namespace Tyra {

MeshBuilderMaterialData::MeshBuilderMaterialData() {
  ambient.set(128.0F, 128.0F, 128.0F, 128.0F);
}
MeshBuilderMaterialData::~MeshBuilderMaterialData() {
  for (auto& frame : frames) {
    delete frame;
  }
}

}  // namespace Tyra
