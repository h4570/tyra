#include "font/font.hpp"
#include <loaders/texture/png_loader.hpp>
#include <malloc.h>

namespace Tyra {

Font::Font() {}
Font::~Font() {
  for (unsigned int i = 0; i < fontData.size(); i++) {
    unloadFont(fontData[i]);
  }
}

void Font::unloadFontTexture(FontData* font) {
  int size;
  sizeInMBused -= font->sizeInMB;
  for (unsigned int i = 0; i < font->size.size(); i++) {
    size = font->texture[i].size();
    for (int j = 0; j < size; j++) {
      font->sizeInMB -= font->texture[i][j]->getSizeInMB();
      renderer->getTextureRepository().free(font->texture[i][j]);
      font->texture[i][j] = nullptr;
      font->texture[i].pop_back();
      font->glyph[i].pop_back();
    }
    font->size[i] = 0;
  }

  size = font->size.size();
  for (int i = 0; i < size; i++) {
    font->size.pop_back();
  }

  if (font->sizeInMB < 0) {
    font->sizeInMB *= -1;
  }
  if (sizeInMBused < 0) {
    sizeInMBused *= -1;
  }
  printf("texture destroy\n");
}

void Font::unloadFont(FontData* font) {
  if (font->face != nullptr) {
    FT_Done_Face(font->face);
    font->face = nullptr;
  }

  unloadFontTexture(font);

  if (font->data) {
    delete font->data;
    font->data = nullptr;
  }

  int size = fontData.size();
  for (int i = 0; i < size; i++) {
    if (font->id == fontData[i]->id) {
      deletedIDs.push_back(font->id);
      fontData.erase(fontData.begin() + i);
      break;
    }
  }

  TYRA_LOG("FONT DELETED");
}

void Font::init(Renderer* t_renderer) {
  renderer = t_renderer;
  error = FT_Init_FreeType(&library);
}

void Font::setLimitFontTexture(float maxMB) {
  TYRA_ASSERT(maxMB >= 1 && maxMB <= 10,
              "Max size in Mb must be between 1 and 10");
  maxSizeInMB = maxMB;
}

int getSizeTexture(const int size) {
  if (size <= 8) {
    return 8;
  } else if (size <= 16) {
    return 16;
  } else if (size <= 32) {
    return 32;
  } else if (size <= 64) {
    return 64;
  } else if (size <= 128) {
    return 128;
  } else if (size <= 256) {
    return 256;
  }
  return 512;
}

TextureBuilderData* getTextureData(const int width, const int height) {
  TextureBuilderData* texData = new TextureBuilderData();
  texData->width = width;
  texData->height = height;
  texData->data =
      new (std::align_val_t(128)) unsigned char[width * height * 4]();
  texData->bpp = TextureBpp::bpp32;
  texData->clutGsComponents = TEXTURE_COMPONENTS_RGBA;

  return texData;
}

int Font::getGlyphTexture(FontData* font, const int fontIndex, int* index,
                          const int codepoint, const int fontSize) {
  FT_GlyphSlot slot = font->face->glyph;

  int flags = FT_LOAD_DEFAULT;
  if (font->hasColor == true) {
    flags |= FT_LOAD_COLOR;
  }

  FT_UInt glyph_index = FT_Get_Char_Index(font->face, codepoint);

  error = FT_Load_Glyph(font->face, glyph_index, flags);
  if (error) {
    printf("Error loading glyph\n");
    return 1;
  }

  error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
  if (error) {
    printf("Error render glyph\n");
    return 2;
  }

  int width = getSizeTexture(slot->bitmap.width);
  int height = getSizeTexture(slot->bitmap.rows);

  *index = font->glyph[fontIndex].size();

  Glyph glyph;
  glyph.value = codepoint;
  glyph.advance = Vec2(slot->advance.x / 64, slot->advance.y / 64);
  glyph.bitmap = Vec2(slot->bitmap_left, slot->bitmap_top);
  glyph.sprite.size = Vec2(width, height);
  font->glyph[fontIndex].push_back(glyph);

  TextureBuilderData* texData = getTextureData(width, height);

  struct PngPixel4* pixels = reinterpret_cast<PngPixel4*>(texData->data);

  int k = 0;
  unsigned char* bitmapPixels = slot->bitmap.buffer;

  if (slot->bitmap.pixel_mode != FT_PIXEL_MODE_BGRA) {
    for (unsigned int i = 0; i < slot->bitmap.rows; i++) {
      for (unsigned int j = 0; j < slot->bitmap.width; j++) {
        pixels[k].r = *bitmapPixels++;
        pixels[k].g = pixels[k].r;
        pixels[k].b = pixels[k].r;
        pixels[k].a = (int)(pixels[k].r * 128 / 255);
        k++;
      }

      k += width - slot->bitmap.width;
    }
  } else {
    for (unsigned int i = 0; i < slot->bitmap.rows; i++) {
      for (unsigned int j = 0; j < slot->bitmap.width; j++) {
        pixels[k].b = *bitmapPixels++;
        pixels[k].g = *bitmapPixels++;
        pixels[k].r = *bitmapPixels++;
        pixels[k].a = (int)(*bitmapPixels++ * 128 / 255);
        k++;
      }

      k += width - slot->bitmap.width;
    }
  }

  font->texture[fontIndex].push_back(new Texture(texData));

  delete texData;
  font->texture[fontIndex][*index]->addLink(
      font->glyph[fontIndex][*index].sprite.id);

  renderer->getTextureRepository().add(font->texture[fontIndex][*index]);

  font->sizeInMB += font->texture[fontIndex][*index]->getSizeInMB();
  sizeInMBused += font->texture[fontIndex][*index]->getSizeInMB();
  return 0;
}

int Font::getCodepoint(const unsigned char* text, int* bytes) {
  /* UTF-8 more info in https://en.wikipedia.org/wiki/UTF-8

    Code point ↔ UTF-8 conversion
    First code and last code point are in hexadecimal.
    If use 2 bytes or more, from the byte 2 they need to be 10xxxxxx until the
    last byte used.
    The "x" means can be 1 or 0.
    |----------------------------------------------------------------------|
    |First code point |Last code point |Byte 1  |Byte 2  |Byte 3  |Byte 4  |
    |(0000-0000)      |(0000-007F)     |0xxxxxxx|nothing |nothing |nothing |
    |(0000-0080)      |(0000-07FF)     |110xxxxx|10xxxxxx|nothing |nothing |
    |(0000-0800)      |(0000-FFFF)     |1110xxxx|10xxxxxx|10xxxxxx|nothing |
    |(0001-0000)      |(0010-FFFF)     |11110xxx|10xxxxxx|10xxxxxx|10xxxxxx|
    |----------------------------------------------------------------------|
    0x80 = 1000-0000
    0xC0 = 1100-0000
    0xE0 = 1110-0000
    0xF0 = 1111-0000
    0xF8 = 1111-1000
  */

  int codepoint = 0x3f;  // char '?'
  *bytes = 1;

  // 0x00 minimum value allowed, 0x80 minimum value NOT allowed
  if (0x00 == (0x80 & text[0])) {
    codepoint = text[0];
  }  // 0xC0 minimum value allowed, 0xE0 minimum value NOT allowed
  else if (0xC0 == (0xE0 & text[0])) {
    if ((text[1] & 0xC0) == 0x80) {  // verify that it is 10xxxxxx.
      codepoint = ((text[0] ^ 0xC0) << 6) | ((text[1] ^ 0x80));
      *bytes = 2;
    }
  }  // 0xE0 minimum value allowed, 0xF0 minimum value NOT allowed
  else if (0xE0 == (0xF0 & text[0])) {
    if ((text[1] & 0xC0) == 0x80 && (text[2] & 0xC0) == 0x80) {
      codepoint = ((text[0] ^ 0xE0) << 12) | ((text[1] ^ 0x80) << 6) |
                  ((text[2] ^ 0x80));
      *bytes = 3;
    }
  }  // 0xF0 minimum value allowed, 0xF8 minimum value NOT allowed
  else if (0xF0 == (0xF8 & text[0])) {
    if ((text[1] & 0xC0) == 0x80 && (text[2] & 0xC0) == 0x80 &&
        (text[3] & 0xC0) == 0x80) {
      codepoint = ((text[0] ^ 0xF0) << 18) | ((text[1] ^ 0x80) << 12) |
                  ((text[2] ^ 0x80) << 6) | ((text[3] ^ 0x80));
      *bytes = 4;
    }
  }

  return codepoint;
}

int Font::getCodepoint(const char* text, int* bytes) {
  return getCodepoint(reinterpret_cast<const unsigned char*>(&text[0]), bytes);
}

int Font::getGlyphIndex(std::vector<Tyra::Glyph>* glyph, const int codepoint) {
  for (unsigned int i = 0; i < glyph->size(); i++) {
    if (codepoint == (*glyph)[i].value) {
      return i;
    }
  }
  return glyphNotFound;
}

int Font::getFontSizeIndex(FontData* font, const int fontSize) {
  if (textureLimits == true) {
    if (sizeInMBused >= maxSizeInMB) {
      for (unsigned int i = 0; i < fontData.size(); i++) {
        unloadFontTexture(fontData[i]);
      }
    }
  }

  for (unsigned int i = 0; i < font->size.size(); i++) {
    if (fontSize == font->size[i]) {
      return i;
    }
  }

  TYRA_ASSERT(fontSize >= 0, "Font size can't be negative");

  int error = FT_Set_Pixel_Sizes(font->face, 0, fontSize);
  TYRA_ASSERT(
      error == 0,
      "Font can't change size, maybe the font was deleted or didn't load.");

  std::vector<Glyph> newGliph;
  std::vector<Texture*> newTexture;

  font->size.push_back(fontSize);
  font->glyph.push_back(newGliph);
  font->texture.push_back(newTexture);
  return font->size.size() - 1;
}

void Font::drawText(FontData* font, std::string text, float x, float y,
                    int fontSize, Color color) {
  int charsUsed;
  int codepoint;
  int index;  // glyph index
  int fontIndex = -1;
  int offsetX = 0;
  int offsetY = 0;
  int previous = 0;  // for kerning

  /*
    The color of the texture is not changed by pixel
    Texture function is modulate.
    So the texture final color is gonna be
    (texture red pixel * color red by user) >> 7
    (texture blue pixel * color blue by user) >> 7
    (texture green pixel * color green by user) >> 7
    (texture alpha pixel * color alpha by user) >> 7
    Sometimes it will have the approximate color
  */

  int correctColor = color.r;
  correctColor = correctColor << 7;
  color.r = correctColor / 255;
  correctColor = color.g;
  correctColor = correctColor << 7;
  color.g = correctColor / 255;
  correctColor = color.b;
  correctColor = correctColor << 7;
  color.b = correctColor / 255;

  fontIndex = getFontSizeIndex(font, fontSize);

  for (unsigned int i = 0; i < text.length();) {
    if (text[i] == '\n') {
      offsetY += fontSize;
      offsetX = 0.0f;
    } else if (text[i] != ' ' && text[i] != '\t') {
      codepoint = getCodepoint(reinterpret_cast<const unsigned char*>(&text[i]),
                               &charsUsed);
      index = getGlyphIndex(&font->glyph[fontIndex], codepoint);

      if (index == glyphNotFound) {
        getGlyphTexture(font, fontIndex, &index, codepoint, fontSize);
      }

      if (font->hasKerning && previous && index) {
        FT_Vector delta;

        FT_Get_Kerning(font->face, previous, index, FT_KERNING_DEFAULT, &delta);

        offsetX += delta.x >> 6;
      }

      font->glyph[fontIndex][index].sprite.color = color;
      font->glyph[fontIndex][index].sprite.position = Vec2(
          x + offsetX, y + offsetY - font->glyph[fontIndex][index].bitmap.y);

      renderer->renderer2D.render(font->glyph[fontIndex][index].sprite);
      offsetX += font->glyph[fontIndex][index].advance.x -
                 font->glyph[fontIndex][index].bitmap.x;
      previous = index;
    } else {
      offsetX += 8 * fontSize / 32;
      if (text[i] == '\t') {
        offsetX += (8 * fontSize / 32) * 2;
      }
    }

    i += charsUsed;
  }
}

void Font::loadFontGlyphs(FontData* font, const int fontSize,
                          std::vector<int>& codePoints,
                          const std::string& filePath) {
  FILE* file = fopen(filePath.c_str(), "rb");
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  fseek(file, 0, SEEK_SET);

  font->data = reinterpret_cast<FT_Byte*>(malloc(size));

  fread(font->data, 1, size, file);

  fclose(file);

  error = FT_New_Memory_Face(library, font->data, size, 0, &font->face);

  TYRA_ASSERT(
      error != FT_Err_Unknown_File_Format,
      "The font file could be opened and read, but it appears, that its font "
      "format is unsupported");

  font->hasKerning = FT_HAS_KERNING(font->face);

  error = FT_Set_Pixel_Sizes(font->face, 0, fontSize);

  if (deletedIDs.empty() == false) {
    font->id = deletedIDs.front();
    deletedIDs.erase(deletedIDs.begin());
  } else {
    font->id = fontData.size();
  }

  if (FT_HAS_COLOR(font->face) == false) {
    TYRA_LOG("Font has no color");
  } else {
    TYRA_LOG("Font has color");
    font->hasColor = true;
  }

  int fontIndex = getFontSizeIndex(font, fontSize);
  int index;

  for (unsigned int i = 0; i < codePoints.size(); i++) {
    getGlyphTexture(font, fontIndex, &index, codePoints[i], fontSize);
  }

  fontData.push_back(font);
  TYRA_LOG("Font Loaded!");
}

void Font::loadFont(FontData* font, const int fontSize,
                    const std::string& filePath) {
  std::vector<int> asciiCodepoints;
  for (int i = 32; i < 128; i++) {
    asciiCodepoints.push_back(i);
  }
  loadFontGlyphs(font, fontSize, asciiCodepoints, filePath);
}

void Font::loadFont(FontData* font, const std::string& filePath) {
  loadFont(font, defaultFontSize, filePath);
}

}  // namespace Tyra