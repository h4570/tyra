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

class FontData {
 public:
  int id;
  float sizeInMB = 0;
  bool hasKerning = false;
  bool hasColor = false;
  std::vector<int> size;
  std::vector<std::vector<Glyph>> glyph;
  std::vector<std::vector<Texture*>> texture;
  FT_Face face;  // handle to face object
  FT_Byte* data;
};

class Font {
 public:
  Font();
  ~Font();
  void init(Renderer* renderer);

  /**
   * Changes the maximum memory size for all fonts.
   * @param maxMB Allowed value between 1 and 10.
   * When the MB is greater than this value, it deletes all cached textures from
   * all font. By default is 2MB.
   */
  void setLimitFontTexture(float maxMB);

  /**
   * @return Only 1 codepoint
   */
  int getCodepoint(const unsigned char* text, int* bytes);

  /**
   * @return Only 1 codepoint
   */
  int getCodepoint(const char* text, int* bytes);
  /**
   * Loads a range of codepoint values.
   * @param codePoints Use getCodepoint() to get the value.
   */
  void loadFontGlyphs(FontData* font, const int fontSize,
                      std::vector<int>& codePoints,
                      const std::string& filePath);
  /**
   * Load the size you require.
   * Loads chars values with 32-127
   */
  void loadFont(FontData* font, const int fontSize,
                const std::string& filePath);

  /**
   * Loads the font with the size of 32 by default.
   * It is recommended to put the size you require
   * so that it loads faster.
   * Loads chars values with 32-127
   */
  void loadFont(FontData* font, const std::string& filePath);

  void drawText(FontData* font, std::string text, float x, float y,
                int fontSize, Color color);
  void unloadFont(FontData* font);
  void unloadFontTexture(FontData* font);

  bool textureLimits = true;

 private:
  Renderer* renderer;
  FT_Library library; /* handle to library     */
  FT_UInt defaultFontSize = 32;
  FT_Error error;
  int glyphNotFound = -1;
  float maxSizeInMB = 2;  // Max size in MB for textures fonts.
  float sizeInMBused = 0;
  std::vector<int> deletedIDs;
  std::vector<FontData*> fontData;

  int getFontSizeIndex(FontData* font, const int fontSize);
  int getGlyphIndex(std::vector<Tyra::Glyph>* glyph, const int codepoint);
  int getGlyphTexture(FontData* font, const int fontIndex, int* index,
                      const int codepoint, const int fontSize);
};
}  // namespace Tyra