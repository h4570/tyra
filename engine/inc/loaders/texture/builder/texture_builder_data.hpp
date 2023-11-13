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

#include <tamtypes.h>
#include <string>
#include "renderer/core/texture/models/texture_bpp.hpp"

namespace Tyra {

class TextureBuilderData {
 public:
  TextureBuilderData();
  ~TextureBuilderData();

  std::string name;

  int width, height;
  u8* data;
  TextureBpp bpp;
  u8 gsComponents;

  int clutWidth, clutHeight;
  u8* clut;
  TextureBpp clutBpp;
  u8 clutGsComponents;
};

}  // namespace Tyra
