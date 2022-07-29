/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/2d/renderer_2d.hpp"

namespace Tyra {

Renderer2D::Renderer2D() {}
Renderer2D::~Renderer2D() {}

void Renderer2D::init(RendererCore* t_rendererCore) { core = t_rendererCore; }

void Renderer2D::render(Sprite* sprite) {
  auto* texture = core->texture.repository.getBySpriteOrMesh(sprite->getId());
  auto texBuffers = core->texture.useTexture(texture);
  core->texture.updateClutBuffer(texBuffers.clut);
  core->renderer2D.render(sprite, texBuffers, texture);
}

}  // namespace Tyra
