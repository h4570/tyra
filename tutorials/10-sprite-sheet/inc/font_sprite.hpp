/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022-2023, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Guido Diego Quispe Robles
*/

#pragma once

#include <tyra>

#define FONT_CHAR_SIZE 96

namespace Tyra {

class Font {
 public:
  Font();
  void load(TextureRepository& repository, Renderer2D* renderer);
  void free(TextureRepository& repository);
  void drawText(const char* text, const int& x, const int& y, Color color);
  void drawText(const std::string& text, const int& x, const int& y,
                Color color);

 private:
  const static int chars[FONT_CHAR_SIZE];
  const static int charWidths[FONT_CHAR_SIZE];

  Renderer2D* renderer2D;
  Sprite allFont;
  std::array<Sprite, FONT_CHAR_SIZE> font;
};

}  // namespace Tyra