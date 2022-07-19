/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <string>
#include "debug/debug.hpp"
#include "loaders/texture/base/texture_loader.hpp"

namespace Tyra {

u32 TextureLoader::getTextureSize(const u32& width, const u32& height,
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
