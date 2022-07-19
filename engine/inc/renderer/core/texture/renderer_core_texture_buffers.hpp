/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <tamtypes.h>
#include <draw_buffers.h>

namespace Tyra {

struct RendererCoreTextureBuffers {
  /** Texture id */
  u32 id;

  /**
   * Texture data.
   * Used in: 4bpp, 8bpp, 24bpp, 32bpp.
   */
  texbuffer_t* core;

  /**
   * Texture pallete data.
   * Used in: 4bpp, 8bpp (otherwise nullptr).
   */
  texbuffer_t* clut;
};

}  // namespace Tyra