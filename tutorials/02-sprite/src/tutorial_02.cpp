/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tyra>
#include "tutorial_02.hpp"

namespace Tyra {

Tutorial02::Tutorial02(Engine* t_engine) : engine(t_engine) {
  texture = nullptr;
}

Tutorial02::~Tutorial02() {
  if (texture != nullptr) {
    engine->renderer.core.texture.repository.free(texture);
  }
}

void Tutorial02::init() {
  engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));

  loadSprite();
  loadTexture();
}

void Tutorial02::loop() {
  auto& renderer = engine->renderer;

  renderer.beginFrame();
  renderer.renderer2D.render(sprite);
  renderer.endFrame();
}

void Tutorial02::loadSprite() {
  const auto& screenSettings = engine->renderer.core.getSettings();

  sprite.mode = SpriteMode::MODE_STRETCH;
  sprite.size = Vec2(256.0F, 128.0F);
  sprite.position =
      Vec2(screenSettings.getWidth() / 2.0F - sprite.size.x / 2.0F,
           screenSettings.getHeight() / 2.0F - sprite.size.y / 2.0F);

  TYRA_LOG("Sprite created!");
}

void Tutorial02::loadTexture() {
  auto& renderer = engine->renderer;
  auto& rendererCore = renderer.core;
  auto& textureRepository = rendererCore.texture.repository;

  auto filepath = FileUtils::fromCwd("tyra.png");
  texture = textureRepository.add(filepath);

  texture->addLink(sprite.id);

  TYRA_LOG("Texture loaded!");
}

}  // namespace Tyra
