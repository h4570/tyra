/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/2d/renderer_2d.hpp"

namespace Tyra {

Renderer2D::Renderer2D() {}
Renderer2D::~Renderer2D() {}

void Renderer2D::init(RendererCore* t_rendererCore) { core = t_rendererCore; }

void Renderer2D::render(const Sprite* sprite) { render(*sprite); }

void Renderer2D::render(const Sprite& sprite) {
  auto* texture = core->texture.repository.getBySpriteId(sprite.id);

  TYRA_ASSERT(
      texture, "Texture for sprite with id: ", sprite.id,
      "Was not found in texture repository! Did you forget to add texture?");

  auto texBuffers = core->texture.useTexture(texture);
  core->texture.updateClutBuffer(texBuffers.clut);
  core->renderer2D.render(sprite, texBuffers, texture);
}

}  // namespace Tyra
