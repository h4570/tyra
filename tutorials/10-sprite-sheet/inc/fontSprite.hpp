/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <tyra>

namespace Tyra{

extern const int charSize;
extern const int font_char[96];
extern const int font_charWidth[96];

class Font{
 public:
  Font();
  void LoadFont(TextureRepository& repository, Renderer2D* renderer);
  void FreeFont(TextureRepository& repository);
  void DrawText(const char* text, int x, int y, Color color);
  void DrawText(const std::string& text, int x, int y, Color color);
 private:
  Renderer2D* renderer2D;  
  Sprite all_font;
  std::array<Sprite,96> font;
};

}  // namespace Tyra