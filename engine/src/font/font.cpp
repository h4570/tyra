#include "font/font.hpp"
#include <loaders/texture/png_loader.hpp>
#include <malloc.h>

namespace Tyra {

namespace TyraFont {
FT_Library library; /* handle to library     */
FT_UInt defaultFontSize = 32;
FT_Error error;
int glyphNotFound = -1;
float maxSizeInMB = 2;
float sizeInMBused = 0;

Font defaultFont;
std::vector<Font*> fontLoaded;
std::vector<int> deletedIDs;
Renderer* renderer;
}  // namespace TyraFont

Font::Font() {}
Font::~Font() { unloadFont(this); }

void unloadFontTexture(Font* font) {
  int size;
  TyraFont::sizeInMBused -= font->sizeInMB;
  for (unsigned int i = 0; i < font->size.size(); i++) {
    size = font->texture[i].size();
    for (int j = 0; j < size; j++) {
      font->sizeInMB -= font->texture[i][j]->getSizeInMB();
      TyraFont::renderer->getTextureRepository().free(font->texture[i][j]);
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
  if (TyraFont::sizeInMBused < 0) {
    TyraFont::sizeInMBused *= -1;
  }
}

void unloadFont(Font* font) {
  if (font->face != nullptr) {
    FT_Done_Face(font->face);
    font->face = nullptr;
  }

  unloadFontTexture(font);

  if (font->data) {
    delete font->data;
    font->data = nullptr;
  }

  int size = TyraFont::fontLoaded.size();
  for (int i = 0; i < size; i++) {
    if (font->id == TyraFont::fontLoaded[i]->id) {
      TyraFont::deletedIDs.push_back(font->id);
      TyraFont::fontLoaded.erase(TyraFont::fontLoaded.begin() + i);
      break;
    }
  }

  TYRA_LOG("FONT DELETED");
}

void fontInit(Renderer* t_renderer) { TyraFont::renderer = t_renderer; }

/*void loadDefaulFont() {
  TYRA_LOG("Font loaded");
}*/
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
  texData->bpp = TextureBpp::bpp32;
  texData->gsComponents = TEXTURE_COMPONENTS_RGBA;
  texData->data =
      new (std::align_val_t(128)) unsigned char[width * height * 4]();
  return texData;
}

int getGlyphTexture(Font* font, const int fontIndex, int* index,
                    const int codepoint, const int fontSize) {
  FT_GlyphSlot slot = font->face->glyph;
  /* load glyph image into the slot (erase previous one) */
  TyraFont::error = FT_Load_Char(font->face, codepoint, FT_LOAD_RENDER);
  if (TyraFont::error) {
    printf("Error loading glyph\n");
    return 1;
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
  for (unsigned int i = 0; i < slot->bitmap.rows; i++) {
    for (unsigned int j = 0; j < slot->bitmap.width; j++) {
      pixels[k].r = (u8)slot->bitmap.buffer[(i * slot->bitmap.width) + j];
      pixels[k].g = pixels[k].r;
      pixels[k].b = pixels[k].r;
      pixels[k].a = (int)(pixels[k].r * 128 / 255);
      // if(j==0){
      //   pixels[k].r = 255;
      // pixels[k].g = 0,
      // pixels[k].b = 0;
      // pixels[k].a = 128;
      // }
      k++;
    }
    k += width - slot->bitmap.width;
  }
  // k = 0;
  // for (int i = 0; i < 1; i++) {
  //   for (int j = 0; j < (int)slot->bitmap.width; j++) {
  //     pixels[k].r = 255;
  //     pixels[k].g = 0,
  //     pixels[k].b = 0;
  //     pixels[k].a = 128;
  //     k++;
  //   }
  // }

  font->texture[fontIndex].push_back(new Texture(texData));

  delete texData;
  font->texture[fontIndex][*index]->addLink(
      font->glyph[fontIndex][*index].sprite.id);
  TyraFont::renderer->getTextureRepository().add(
      font->texture[fontIndex][*index]);
  font->sizeInMB += font->texture[fontIndex][*index]->getSizeInMB();
  TyraFont::sizeInMBused += font->texture[fontIndex][*index]->getSizeInMB();
  return 0;
}

int getCodepoint(const unsigned char* text, int* bytes) {
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

int getGlyphIndex(std::vector<Tyra::Glyph>* glyph, const int codepoint) {
  for (unsigned int i = 0; i < glyph->size(); i++) {
    if (codepoint == (*glyph)[i].value) {
      return i;
    }
  }
  return TyraFont::glyphNotFound;
}

int getFontSizeIndex(Font* font, const int fontSize) {
  if (TyraFont::sizeInMBused >= TyraFont::maxSizeInMB) {
    for (unsigned int i = 0; i < TyraFont::fontLoaded.size(); i++) {
      unloadFontTexture(TyraFont::fontLoaded[i]);
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

void drawText(Font* font, std::string text, float x, float y, int fontSize,
              Color color) {
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

      if (index == TyraFont::glyphNotFound) {
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

      TyraFont::renderer->renderer2D.render(
          font->glyph[fontIndex][index].sprite);
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

// For default font
// void drawText(std::string text, float x, float y, int fontSize, Color color)
// {
//   drawText(&defaultFont, text, x, y, fontSize, color);
// }

void setMaxSizeInFontMemory(float maxMB) {
  TYRA_ASSERT(maxMB >= 1 && maxMB <= 10,
              "Max size in Mb must be between 1 and 10");
  TyraFont::maxSizeInMB = maxMB;
}

void loadFont(Font* font, const std::string filePath, int fontSize) {
  FT_Error error = FT_Init_FreeType(&TyraFont::library);

  std::ifstream file(filePath, std::ios::binary);
  file.seekg(0, file.end);
  size_t size = file.tellg();
  file.seekg(0, std::ios::beg);

  font->data = reinterpret_cast<FT_Byte*>(malloc(size));

  TYRA_ASSERT(file.is_open(), "The font file could not be opened.");

  file.read(reinterpret_cast<char*>(font->data), size);

  TYRA_ASSERT(!file.fail() && file.gcount() != size,
              "Error trying to read the font file.");

  file.close();

  error =
      FT_New_Memory_Face(TyraFont::library, font->data, size, 0, &font->face);

  TYRA_ASSERT(
      error != FT_Err_Unknown_File_Format,
      "The font file could be opened and read, but it appears, that its font "
      "format is unsupported");

  font->hasKerning = FT_HAS_KERNING(font->face);

  error = FT_Set_Pixel_Sizes(font->face, 0, fontSize);

  if (TyraFont::deletedIDs.empty() == false) {
    font->id = TyraFont::deletedIDs.front();
    TyraFont::deletedIDs.erase(TyraFont::deletedIDs.begin());
  } else {
    font->id = TyraFont::fontLoaded.size();
  }

  int fontIndex = getFontSizeIndex(font, fontSize);
  int index;

  // Load the ASCII Font
  for (int i = 32; i < 128; i++) {
    getGlyphTexture(font, fontIndex, &index, i, fontSize);
  }

  TyraFont::fontLoaded.push_back(font);
  TYRA_LOG("Font Loaded!");
}

void loadFont(Font* font, const std::string filePath) {
  loadFont(font, filePath.c_str(), TyraFont::defaultFontSize);
}

}  // namespace Tyra