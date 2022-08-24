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

#include "loaders/loader.hpp"
#include "loaders/texture/builder/texture_builder_data.hpp"
#include "renderer/core/texture/models/texture_bpp.hpp"
#include <string>

namespace Tyra {
class TextureLoader : public Loader {
 public:
  /**
   * @brief Loads texture data from file.
   * Sets size, name and data.
   * @param fullpath Full path to texture file. Example: "host:texture.png"
   */
  virtual TextureBuilderData* load(const char* fullpath) = 0;

  inline TextureBuilderData* load(const std::string& fullpath) {
    return load(fullpath.c_str());
  }

  u32 getTextureSize(const u32& width, const u32& height,
                     const TextureBpp& bpp);
};
}  // namespace Tyra
