#pragma once

#include <loaders/texture/builder/texture_builder_data.hpp>
#include <renderer/core/texture/models/texture.hpp>
#include <renderer/core/2d/sprite/sprite.hpp>
#include <renderer/renderer.hpp>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Tyra {

struct Glyph {
  int value = 0;  // codepoint value
  Vec2 advance;
  Vec2 bitmap;  // bearing
  Sprite sprite;
};

class Font {
 public:
  Font();
  ~Font();
  int id;
  float sizeInMB = 0;
  bool hasKerning = false;
  std::vector<int> size;
  std::vector<std::vector<Glyph>> glyph;
  std::vector<std::vector<Texture*>> texture;
  FT_Face face;  // handle to face object
  FT_Byte* data;
};

extern void fontInit(Renderer* renderer);

/**
 * Changes the maximum memory size for all fonts.
 * @param maxMB Allowed value between 1 and 10.
 * When the MB is greater than this value, it deletes all cached textures from
 * all font. By default is 2MB.
 */
extern void setMaxSizeInFontMemory(float maxMB);

/*
 * Loads the font with the size you require
 */
extern void loadFont(Font* font, std::string filePath, int fontSize);

/*
 * Loads the font with the size of 32 by default.
 * It is recommended to put the size you require first so that it loads faster.
 */
extern void loadFont(Font* font, std::string filePath);
// extern void loadDefaulFont();
extern void unloadFont(Font* font);
extern void drawText(Font* font, std::string text, float x, float y,
                      int fontSize, Color color);
// extern void drawText(std::string text, float x, float y, int fontSize,
//                       Color color); // For default font
}  // namespace Tyra