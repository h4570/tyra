/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <string>

namespace Tyra {

/** Class responsible for loading&parsing Quake's II ".md2" 3D files */
class MD2Loader : public Loader {
 public:
  MD2Loader();
  ~MD2Loader();

  MeshBuilderData* load(const char* fullpath, const float& t_scale,
                        const u8& t_invertT);

  inline MeshBuilderData* load(const std::string& fullpath,
                               const float& t_scale, const u8& t_invertT) {
    return load(fullpath.c_str(), t_scale, t_invertT);
  }
};

}  // namespace Tyra
