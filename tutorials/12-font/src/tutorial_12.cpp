/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Guido Diego Quispe Robles
*/

#include <tyra>
#include "tutorial_12.hpp"

namespace Tyra {

Tutorial12::Tutorial12(Engine* t_engine) : engine(t_engine) {}

Tutorial12::~Tutorial12() {
  /**
   * It is not necessary to use it here,
   * it is destroyed when the font is
   * destroyed but in case you stop using it,
   * is good to use it to free up memory.
   */
  engine->font.unloadFont(&myFont);
}

void Tutorial12::init() {
  engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));
  // engine->font.setLimitFontTexture(2.0f); // by default 2.0MB
  // engine->font.textureLimits = false; // disable texture limit.

  /** How to load the texture of an individual character.
   *
   * ¡Warning: Don't forget that you have a limit for font textures,
   * if you exceed the limit they will be deleted.!
   */

  int count;
  int codePoint = engine->font.getCodepoint("🥳", &count);
  std::vector<int> codePoints;
  codePoints.push_back(codePoint);
  engine->font.loadFontGlyphs(&myEmojiFont, 32, codePoints,
                              FileUtils::fromCwd("seguiemj.ttf"));

  /** Loads chars with values 32-127 with a font size of 32
   *
   * If you use a char with a value that is not between 32 and 127
   * the DrawText function will find the char anyway
   * (in case the char is inside on the font).
   *
   * But it is recommended to use these functions to load the
   * chars faster,
   * or else it will take more time to draw because the texture will be created.
   *
   * If the texture limit is exceeded or change the size of the font in
   * DrawText, the above will happen.
   */

  engine->font.loadFont(&myFont, FileUtils::fromCwd("roboto-bold.ttf"));
  // engine->font.loadFont(&myFont, 32, FileUtils::fromCwd("roboto-bold.ttf"));

  white = Color(255.0f, 255.0f, 255.0f, 128.0f);
  skyBlue = Color(32.0f, 164.0f, 243.0f, 128.0f);
}

void Tutorial12::loop() {
  auto& renderer = engine->renderer;
  auto& font = engine->font;

  /** Begin frame will clear our screen. */
  renderer.beginFrame();

  /** Render font. */
  /** It uses the freetype library so it supports truetype and opentype fonts.
   */

  font.drawText(&myFont, "Hello world in", 20, 60, 32, white);
  font.drawText(&myFont, "TYRA", 212, 60, 32, skyBlue);
  font.drawText(&myEmojiFont, "🥳", 300, 60, 32, white);

  /** End frame will perform vsync. */
  renderer.endFrame();
}

}  // namespace Tyra
