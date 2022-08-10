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
#include "../builder/mesh_builder_data.hpp"
#include <string>

namespace Tyra {

struct MD2LoaderOptions {
  bool flipUVs = false;
  float scale = 1.0F;
};

/** Class responsible for loading & parsing Quake's II ".md2" 3D files */
class MD2Loader : public Loader {
 public:
  MD2Loader();
  ~MD2Loader();

  MeshBuilderData* load(const char* fullpath);
  MeshBuilderData* load(const char* fullpath, MD2LoaderOptions options);
  MeshBuilderData* load(const std::string& fullpath);
  MeshBuilderData* load(const std::string& fullpath, MD2LoaderOptions options);
};

}  // namespace Tyra
