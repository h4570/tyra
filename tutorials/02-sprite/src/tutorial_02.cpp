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
  /** Lets set default value, to prevent nullptr bugs */
  texture = nullptr;
}

Tutorial02::~Tutorial02() {
  if (texture != nullptr) {
    /**
     * free() function in texture repository is
     * removing texture from repository and calling
     * texture destructor
     */
    engine->renderer.core.texture.repository.free(texture);
  }
}

void Tutorial02::init() {
  engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));

  /** Sprite contains rectangle information. */
  loadSprite();

  /** Texture contains png image. */
  loadTexture();
}

void Tutorial02::loop() {
  auto& renderer = engine->renderer;

  /** Begin frame will clear our screen. */
  renderer.beginFrame();

  /** Render sprite. */
  renderer.renderer2D.render(sprite);

  /** End frame will perform vsync. */
  renderer.endFrame();
}

void Tutorial02::loadSprite() {
  const auto& screenSettings = engine->renderer.core.getSettings();

  sprite.mode = SpriteMode::MODE_STRETCH;

  /** Let's scale it down */
  sprite.size = Vec2(256.0F, 128.0F);

  /** Set it in screen center */
  sprite.position =
      Vec2(screenSettings.getWidth() / 2.0F - sprite.size.x / 2.0F,
           screenSettings.getHeight() / 2.0F - sprite.size.y / 2.0F);

  TYRA_LOG("Sprite created!");
}

void Tutorial02::loadTexture() {
  /**
   * Renderer has high layer functions,
   * which allows to render:
   * - Sprite (2D)
   * - Mesh (3D)
   *
   * It uses ONLY low layer functions which are in renderer.core
   */
  auto& renderer = engine->renderer;

  /**
   * TextureRepository is a repository of textures.
   * It is a singleton class, with all game textures.
   * We are linking these textures with sprite's (2D) and mesh (3D) materials.
   */
  auto& textureRepository = renderer.getTextureRepository();

  /**
   * Texture is stored in "res" directory.
   * Content of "res" directory is automatically copied into
   * "bin" directory, which contains our final game.
   *
   * File utils automatically add's device prefix to the path,
   * based on current working directory.
   *
   * In PS2 world:
   * - USB has a "mass:" prefix
   * - Our PC in PS2Link has a "host:" prefix
   * - Our PC in PCSX2 has a "host:" prefix
   */
  auto filepath = FileUtils::fromCwd("tyra.png");

  /**
   * Tyra supports following PNG formats:
   * 32bpp (RGBA)
   * 24bpp (RGB)
   * 8bpp, palletized (RGBA)
   * 4bpp, palletized (RGBA)
   *
   * 8bpp and 4bpp are the fastest.
   * All of these formats can be easily exported via GIMP.
   */
  texture = textureRepository.add(filepath);

  /** Let's assign this texture to sprite. */
  texture->addLink(sprite.id);

  TYRA_LOG("Texture loaded!");
}

}  // namespace Tyra
