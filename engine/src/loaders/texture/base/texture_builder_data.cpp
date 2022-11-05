/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "loaders/texture/builder/texture_builder_data.hpp"
#include <draw_buffers.h>

namespace Tyra {

TextureBuilderData::TextureBuilderData() {
  width = 0;
  height = 0;
  data = nullptr;
  bpp = bpp32;
  gsComponents = TEXTURE_COMPONENTS_RGBA;

  clut = nullptr;
  clutWidth = 0;
  clutHeight = 0;
  clutBpp = bpp32;
  clutGsComponents = TEXTURE_COMPONENTS_RGBA;
}

TextureBuilderData::~TextureBuilderData() {}

}  // namespace Tyra
