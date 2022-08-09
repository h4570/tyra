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

#include "../../loader.hpp"
#include "../builder2/mesh_builder2_data.hpp"
#include <string>

namespace Tyra {

/** Class responsible for loading & parsing Quake's II ".md2" 3D files */
class MD2Loader : public Loader {
 public:
  MD2Loader();
  ~MD2Loader();

  MeshBuilder2Data* load(const char* fullpath, const float& scale,
                         const bool& invertT);

  inline MeshBuilder2Data* load(const std::string& fullpath, const float& scale,
                                const bool& invertT) {
    return load(fullpath.c_str(), scale, invertT);
  }
};

}  // namespace Tyra
