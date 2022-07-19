/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "renderer/core/2d/sprite/sprite.hpp"
#include "renderer/core/renderer_core.hpp"

namespace Tyra {

class Renderer2D {
 public:
  Renderer2D();
  ~Renderer2D();

  void init(RendererCore* rendererCore);
  void render(Sprite* sprite);

 private:
  RendererCore* core;
};

}  // namespace Tyra
