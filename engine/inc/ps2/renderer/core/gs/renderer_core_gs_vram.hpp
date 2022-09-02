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

#include "renderer/renderer_settings.hpp"
#include "renderer/core/texture/models/texture.hpp"

namespace Tyra {

class RendererCoreGSVRam {
 public:
  RendererCoreGSVRam();
  ~RendererCoreGSVRam();

  const float& getFreeSpaceInMB();

  float getSizeInMB(const Texture& texture);
  float getSizeInMB(const TextureData& texData);
  float getSizeInMB(int width, const int& height, const int& psm,
                    const int& alignment);

  int allocate(const TextureData& texData);
  int allocateBuffer(const int& width, const int& height, const int& psm);
  int allocate(const int& width, const int& height, const int& psm,
               const int& alignment);

  /** Free texture, FIFO order */
  void free(const int& address);

 private:
  int getSize(int width, const int& height, const int& psm,
              const int& alignment);

  static constexpr float ptr2MB = 262144.0F;
  bool touched;
  int pointer;
  float cachedFreeSpace;
};

}  // namespace Tyra
