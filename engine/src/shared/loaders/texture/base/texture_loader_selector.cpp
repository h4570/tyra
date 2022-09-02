/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <algorithm>
#include <string>
#include "shared/debug/debug.hpp"
#include "shared/renderer/core/texture/models/texture.hpp"
#include "shared/loaders/texture/base/texture_loader_selector.hpp"

namespace Tyra {

TextureLoaderSelector::TextureLoaderSelector() {}

TextureLoaderSelector::~TextureLoaderSelector() {}

// ----
// Methods
// ----

TextureLoader& TextureLoaderSelector::getLoaderByFileName(
    const char* fullpath) {
  std::string path = fullpath;
  std::string extension = path.substr(path.find_last_of(".") + 1);
  return getLoaderByExtension(extension);
}

TextureLoader& TextureLoaderSelector::getLoaderByExtension(
    const std::string& extension) {
  std::string extensionLower = extension;
  std::transform(extensionLower.begin(), extensionLower.end(),
                 extensionLower.begin(), ::tolower);
  if (extensionLower == "png") {
    return pngLoader;
  } else {
    TYRA_TRAP("There is no texture loader for extension: ", extensionLower);
    return pngLoader;
  }
}

}  // namespace Tyra
