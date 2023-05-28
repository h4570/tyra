#include "fontSprite.hpp"

namespace Tyra{

const int charSize = 96;

const int font_char[charSize]{
    ' ', '!', '"', ' ', '$', '%', ' ', '´', '(', ')', ' ', '+', ',', '-', '.', '/',
    '0', '1', '2', '3', '4', '5', '6', '2', '8', '9', ':', ';', '“', '=', '”', '?',
    ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ', ' ', ' ', ' ', ' ',
    ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '[', '♪', ']', '~', ' '
};

const int font_charWidth[charSize] {
    0,   1,   3,   0,   5,   9,   0,   9,   3,   3,   0,   5,   2,   2,   1,   4,
    4,   2,   4,   4,   5,   4,   4,   4,   4,   4,   1,   2,   4,   5,   4,   4,
    0,   6,   5,   5,   5,   4,   4,   5,   5,   1,   4,   5,   4,   7,   5,   5,
    5,   5,   5,   5,   5,   5,   6,   7,   5,   5,   4,   0,   0,   0,   0,   0,
    0,   5,   4,   4,   4,   4,   3,   4,   4,   1,   2,   4,   1,   7,   4,   4,
    4,   4,   3,   4,   3,   4,   5,   7,   4,   4,   4,   2,   5,   2,   6,   0,
};

Font::Font(){ }


void Font::LoadFont(TextureRepository& repository, Renderer2D* renderer){
    renderer2D = renderer;

    float height = 16.0F;
    float width  = 16.0F;
    
    all_font.mode = MODE_REPEAT;
    all_font.size = Vec2(255,127);

    auto filepath = FileUtils::fromCwd("earthbound-Font.png");
    auto* texture = repository.add(filepath);
    texture->addLink(all_font.id);

    int column = 0;
    int arrow  = 0;

    for(int i=0; i < 96; i++){
        font[i].id     = all_font.id;
        font[i].mode   = MODE_REPEAT;
        font[i].size   = Vec2(width,height);
        font[i].offset = Vec2(width*column,height*arrow);
        column++;
        if(column == 16){
            arrow++;
            column = 0;
        }
    }
}

void Font::FreeFont(TextureRepository& repository){
    repository.freeBySprite(all_font); 
    for(int i=0;i<96;i++){ repository.freeBySprite(font[i]); }
}

void Font::DrawText(const char* text, int x, int y, Color color){
    DrawText(std::string(text),x,y,color);
}

void Font::DrawText(const std::string& text, int x, int y, Color color){
    int sizeText = text.size();

    int offsetY = 0;
    int offsetX = 0;
       
    Sprite spr_font;
    int pos_font;

    for (int i = 0; i < sizeText; i++)
    {
        pos_font = text[i];
        spr_font = font[0];

        for(int j=0; j < charSize; j++){
            if(pos_font == font_char[j]){
                pos_font = j;
                spr_font = font[j];
                spr_font.color = color;
                spr_font.position = Vec2(x + offsetX, y + offsetY);
                break;
            }
        }

        if (pos_font == '\n')
        {
            offsetY += 18;
            offsetX = 0.0f;
        }else{
            if ((pos_font != ' ') && (pos_font != '\t')){
                renderer2D->render(spr_font);   
                offsetX += font_charWidth[pos_font] + 2; 
            }else{
                offsetX += 2;
            }

        }
    }
}

}  // namespace Tyra