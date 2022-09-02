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

#include <string>
#include "renderer/core/texture/models/texture_bpp.hpp"

namespace Tyra {

class TextureBuilderData {
 public:
  TextureBuilderData();
  ~TextureBuilderData();

  std::string name;

  int width, height;
  unsigned char* data;
  TextureBpp bpp;
  unsigned char gsComponents;

  int clutWidth, clutHeight;
  unsigned char* clut;
  TextureBpp clutBpp;
  unsigned char clutGsComponents;
};
}  // namespace Tyra
