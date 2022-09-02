/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <string>
#include "shared/debug/debug.hpp"
#include "shared/loaders/texture/base/texture_loader.hpp"

namespace Tyra {

unsigned int TextureLoader::getTextureSize(const unsigned int& width,
                                           const unsigned int& height,
                                           const TextureBpp& bpp) {
  switch (bpp) {
    case bpp32:
      return (width * height * 4);
    case bpp24:
      return (width * height * 3);
    case bpp8:
      return (width * height);
    case bpp4:
      return (width * height / 2);
    default:
      TYRA_TRAP("Unknown texture bpp");
  }

  return -1;
}

}  // namespace Tyra
