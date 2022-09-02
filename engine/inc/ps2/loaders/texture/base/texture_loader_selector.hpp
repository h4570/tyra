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

#include "loaders/texture/base/texture_loader.hpp"
#include "loaders/texture/png_loader.hpp"
#include <string>

namespace Tyra {
class TextureLoaderSelector {
 public:
  TextureLoaderSelector();
  ~TextureLoaderSelector();

  TextureLoader& getLoaderByFileName(const char* fullpath);

  inline TextureLoader& getLoaderByFileName(const std::string& fullpath) {
    return getLoaderByFileName(fullpath.c_str());
  }

  TextureLoader& getLoaderByExtension(const std::string& extension);

 private:
  PngLoader pngLoader;
};
}  // namespace Tyra
