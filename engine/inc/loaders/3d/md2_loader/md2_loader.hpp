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

#include "../../loader.hpp"
#include "../builder/mesh_builder_data.hpp"
#include <string>
#include <memory>

namespace Tyra {

struct MD2LoaderOptions {
  bool flipUVs = false;
  float scale = 1.0F;
};

/** Class responsible for loading & parsing Quake's II ".md2" 3D files */
class MD2Loader : public Loader {
 public:
  static std::unique_ptr<MeshBuilderData> load(const char* fullpath);
  static std::unique_ptr<MeshBuilderData> load(const char* fullpath,
                                               MD2LoaderOptions options);
  static std::unique_ptr<MeshBuilderData> load(const std::string& fullpath);
  static std::unique_ptr<MeshBuilderData> load(const std::string& fullpath,
                                               MD2LoaderOptions options);
};

}  // namespace Tyra
