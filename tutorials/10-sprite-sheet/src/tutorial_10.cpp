/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022-2023, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Guido Diego Quispe Robles
*/

#include <tyra>
#include "tutorial_10.hpp"

namespace Tyra {

Tutorial10::Tutorial10(Engine* t_engine)
    : padTimer(0), engine(t_engine), pad(&t_engine->pad) {}

Tutorial10::~Tutorial10() {
  for (int i = 0; i < 4; i++)
    engine->renderer.getTextureRepository().freeBySprite(sprite[i]);
}

void Tutorial10::init() {
  engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));

  /** Sprite contains rectangle information. */
  loadSprite();

  posX1 = sprite[normal].position.x;
  posX2 = sprite[flip].position.x;
  posX3 = sprite[stretch].position.x;
  posX4 = sprite[scale].position.x;

  posY1 = sprite[normal].position.y +
          (sprite[normal].size.y * sprite[normal].scale) + 16;
  posY2 =
      sprite[flip].position.y + (sprite[flip].size.y * sprite[flip].scale) + 16;
  posY3 = sprite[stretch].position.y +
          (sprite[stretch].size.y * sprite[stretch].scale) + 16;
  posY4 = sprite[scale].position.y +
          (sprite[scale].size.y * sprite[scale].scale) + 16;

  /** Texture contains png image. */
  loadTexture();

  engine->font.loadFont(&myFont, FileUtils::fromCwd("Roboto-Black.ttf"));

  white = Color(255.0F, 255.0F, 255.0F, 128.0F);

  /**  Set the texture filtering. */
  textureFilter = TyraNearest;

  engine->renderer.core.renderer2D.setTextureMappingType(textureFilter);
}

void Tutorial10::loop() {
  auto& renderer = engine->renderer;
  auto& font = engine->font;

  /** Change the texture filtering of the texture.
   *
   *  The options are:
   *  TyraNearest (Point Sampling):
   *  uses the color of the texel closest to the pixel center for the pixel
   * color.
   *
   *  TyraLinear  (Bilinear Sampling):
   *  the four nearest texels to the pixel center are sampled
   *  and their colors are combined by weighted average according to distance.
   *
   *  By default it is linear.
   */

  renderer.core.renderer2D.setTextureMappingType(textureFilter);

  handlePad();

  /** Begin frame will clear our screen. */
  renderer.beginFrame();

  /** Render sprite. */
  renderer.renderer2D.render(sprite);
  renderer.core.renderer2D.setTextureMappingType(TyraLinear);
  renderer.renderer2D.render(sprite[flip]);
  renderer.core.renderer2D.setTextureMappingType(textureFilter);
  renderer.renderer2D.render(sprite[scale]);
  renderer.renderer2D.render(sprite[stretch]);

  renderer.core.renderer2D.setTextureMappingType(TyraLinear);
  font.drawText(&myFont, "tests with sprite sheet", (512 / 2) - 80, 20, 16, white);
  font.drawText(&myFont, "Press Cross for nearest filter", (512 / 2) - 95, 50, 16,
           white);
  font.drawText(&myFont, "Press Circle for linear filter", (512 / 2) - 90, 70, 16,
           white);
  font.drawText(&myFont, "Use left stick for move the offsets of the sprites",
           (512 / 2) - 150, 90, 16, white);
  font.drawText(&myFont, "sprite 32x32\n   scaled x3", sprite[normal].position.x,
           sprite[normal].position.y - 40, 16, white);
  font.drawText(&myFont, "sprite 32x32\nscaled x3 with\n  \t\tflip",
           sprite[flip].position.x, sprite[flip].position.y - 40, 16, white);
  font.drawText(&myFont, "sprite 96x96\n  \t\twith\nrepeat mode",
           sprite[scale].position.x, sprite[scale].position.y - 40, 16, white);
  font.drawText(&myFont, "sprite 96x96\n  \t\twith\nstretch mode",
           sprite[stretch].position.x, sprite[stretch].position.y - 40, 16,
           white);

  if (textureFilter == TyraLinear) {
    strFilter = "Filter: Linear";
  } else {
    strFilter = "Filter: Nearest";
  }

  font.drawText(&myFont, strFilter, posX1, posY1, 16, white);
  font.drawText(&myFont, "Filter: Linear", posX2, posY2, 16, white);
  font.drawText(&myFont, strFilter, posX3, posY3, 16, white);
  font.drawText(&myFont, strFilter, posX4, posY4, 16, white);

  if (offsetIsNegative == true) {
    font.drawText(&myFont,
             "The offset must not be negative or you will get this error", 20,
             posY1 + 32, 16, Color(255.0F, 0, 0, 128));
  }

  /** End frame will perform vsync. */
  renderer.endFrame();
}

void Tutorial10::handlePad() {
  if (engine->pad.getClicked().Cross) {
    textureFilter = TyraNearest;
  } else if (engine->pad.getClicked().Circle) {
    textureFilter = TyraLinear;
  }

  /** Move the offset of the sprites.
   *
   *  The offset of the sprite need to be equal or greater to zero
   *  or a graphical error occurs.
   *
   *  And the sum of the sprite size and the offset must be less than 1024
   *  or a graphical error occurs.
   *
   *  Sum with repeat mode:
   *  Max = sprite.size + sprite.offset.
   *
   *  Sum with strecth mode:
   *  Max = original size of the image + sprite.offset.
   *
   *  If it continues moving past these values
   *  in a moment it will return to normal but
   *  the error will be repeated with another value.
   */

  if (padTimer <= 0) {
    if (pad->getLeftJoyPad().v <= 100) {  // up
      sprite[normal].offset -= Vec2(0, 16);
      sprite[flip].offset -= Vec2(0, 16);
      sprite[scale].offset -= Vec2(0, 16);
      sprite[stretch].offset -= Vec2(0, 16);
      padTimer = 20;
      printf("New offset: %f,%f\n", sprite[normal].offset.x,
             sprite[normal].offset.y);
    } else if (pad->getLeftJoyPad().v >= 200) {  // down
      sprite[normal].offset += Vec2(0, 16);
      sprite[flip].offset += Vec2(0, 16);
      sprite[scale].offset += Vec2(0, 16);
      sprite[stretch].offset += Vec2(0, 16);
      padTimer = 20;
      printf("New offset: %f,%f\n", sprite[normal].offset.x,
             sprite[normal].offset.y);
    }

    if (pad->getLeftJoyPad().h <= 100) {  // left
      sprite[normal].offset -= Vec2(16, 0);
      sprite[flip].offset -= Vec2(16, 0);
      sprite[scale].offset -= Vec2(16, 0);
      sprite[stretch].offset -= Vec2(16, 0);
      padTimer = 20;
      printf("New offset: %f,%f\n", sprite[normal].offset.x,
             sprite[normal].offset.y);
    } else if (pad->getLeftJoyPad().h >= 200) {  // right
      sprite[normal].offset += Vec2(16, 0);
      sprite[flip].offset += Vec2(16, 0);
      sprite[scale].offset += Vec2(16, 0);
      sprite[stretch].offset += Vec2(16, 0);
      padTimer = 20;
      printf("New offset: %f,%f\n", sprite[normal].offset.x,
             sprite[normal].offset.y);
    }

    if (sprite[normal].offset.x < 0 || sprite[normal].offset.y < 0) {
      offsetIsNegative = true;
    } else {
      offsetIsNegative = false;
    }

  } else {
    padTimer--;
  }
}

void Tutorial10::loadSprite() {
  const auto& screenSettings = engine->renderer.core.getSettings();

  int offset = 25;

  for (int i = 0; i < 4; i++) {
    sprite[i].position =
        Vec2((offset * (i + 1)) + ((32.0F * 3) * i),
             screenSettings.getHeight() / 2.0F - sprite[normal].size.y / 2.0F);
    if (i == normal || i == flip) {
      sprite[i].size = Vec2(32.0F, 32.0F);
      sprite[i].scale = 3;
    } else {
      sprite[i].size = Vec2(96.0F, 96.0F);
    }
  }

  sprite[stretch].mode = SpriteMode::MODE_STRETCH;

  sprite[flip].flipHorizontal = true;
  sprite[flip].flipVertical = true;

  TYRA_LOG("Sprite created!");
}

void Tutorial10::loadTexture() {
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
  auto filepath = FileUtils::fromCwd("atlas.png");

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
  auto* texture = textureRepository.add(filepath);

  /** Let's assign this texture to sprite. */
  texture->addLink(sprite[normal].id);

  sprite[flip].id = sprite[normal].id;
  sprite[scale].id = sprite[normal].id;
  sprite[stretch].id = sprite[normal].id;

  TYRA_LOG("Texture loaded!");
}
}  // namespace Tyra
