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
#include "tutorial_10.hpp"

namespace Tyra {

Tutorial10::Tutorial10(Engine* t_engine) : engine(t_engine), pad(&t_engine->pad){ }

Tutorial10::~Tutorial10() {
  engine->renderer.getTextureRepository().freeBySprite(sprite);
  engine->renderer.getTextureRepository().freeBySprite(spriteFlip);
  engine->renderer.getTextureRepository().freeBySprite(spriteScale);
  engine->renderer.getTextureRepository().freeBySprite(spriteStrech);
  font.FreeFont(engine->renderer.getTextureRepository());
}

void Tutorial10::init() {
  engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));

  /** Load all the sprites of the sprite sheet. */
  font.LoadFont(engine->renderer.getTextureRepository(),&engine->renderer.renderer2D);

  /** Sprite contains rectangle information. */
  loadSprite();

  /** Texture contains png image. */
  loadTexture();

  /**  Set the texture filtering. */
  textureFilter = TyraNearest;

  engine->renderer.core.renderer2D.setTextureMappingType(textureFilter);

}

void Tutorial10::loop() {
  auto& renderer = engine->renderer;

  /** Change the texture filtering of the texture.
   * 
   *  The options are:
   *  TyraNearest (Point Sampling):
   *  uses the color of the texel closest to the pixel center for the pixel color. 
   *  
   *  TyraLinear  (Bilinear Sampling):
   *  the four nearest texels to the pixel center are sampled
   *  and their colors are combined by weighted average according to distance.
   * 
   *  By default it is linear.
  */

  renderer.core.renderer2D.setTextureMappingType(textureFilter);
  
  if(engine->pad.getClicked().Cross){
    textureFilter = TyraNearest;
  }else if(engine->pad.getClicked().Circle){
    textureFilter = TyraLinear;
  }

  static int timer = 0;

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

  if(timer <= 0){
    if(pad->getLeftJoyPad().v <= 100){ //up
      sprite.offset -= Vec2(0,16); 
      spriteFlip.offset -= Vec2(0,16);
      spriteScale.offset -= Vec2(0,16);
      spriteStrech.offset -= Vec2(0,16); timer = 20; 
      printf("offset: %f,%f\n",sprite.offset.x,sprite.offset.y);
    }else if(pad->getLeftJoyPad().v >= 200 ){ //down
      sprite.offset += Vec2(0,16); 
      spriteFlip.offset += Vec2(0,16);
      spriteScale.offset += Vec2(0,16);
      spriteStrech.offset += Vec2(0,16); timer = 20; 
      printf("offset: %f,%f\n",sprite.offset.x,sprite.offset.y);
    }

    if (pad->getLeftJoyPad().h <= 100) { // left
       sprite.offset -= Vec2(16,0); 
       spriteFlip.offset -= Vec2(16,0);
       spriteScale.offset -= Vec2(16,0);
       spriteStrech.offset -= Vec2(16,0); timer = 20; 
       printf("offset: %f,%f\n",sprite.offset.x,sprite.offset.y);
    } else if (pad->getLeftJoyPad().h >= 200 ) { //right
       sprite.offset += Vec2(16,0); 
       spriteFlip.offset += Vec2(16,0);
       spriteScale.offset += Vec2(16,0);
       spriteStrech.offset += Vec2(16,0); timer = 20; 
       printf("offset: %f,%f\n",sprite.offset.x,sprite.offset.y);
    }
    
  }else{
    timer--;
  }
  
  /** Begin frame will clear our screen. */
  renderer.beginFrame();

  /** Render sprite. */
  renderer.renderer2D.render(sprite);
  renderer.core.renderer2D.setTextureMappingType(TyraLinear);
  renderer.renderer2D.render(spriteFlip); 
  renderer.core.renderer2D.setTextureMappingType(textureFilter);
  renderer.renderer2D.render(spriteScale);
  renderer.renderer2D.render(spriteStrech);

  font.DrawText("tests with sprite sheet",(512/2)-60,10,Color(255,255,255));
  font.DrawText("Press Cross for nearest filter",(512/2)-70,50,Color(255,255,255));
  font.DrawText("Press Circle for linear filter",(512/2)-65,70,Color(255,255,255));
  font.DrawText("Use left stick for move the offsets of the sprites",(512/2)-110,90,Color(255,255,255));
  font.DrawText("sprite 32x32\n   scaled x3",sprite.position.x+16,sprite.position.y-40,Color(255,255,255));
  font.DrawText("sprite 32x32\nscaled x3 with flip",spriteFlip.position.x+4,spriteFlip.position.y-40,Color(255,255,255)); 
  font.DrawText("sprite 96x96 with\n   repeat mode",spriteScale.position.x+6,spriteScale.position.y-40,Color(255,255,255));
  font.DrawText("sprite 96x96 with\n   stretch mode",spriteStrech.position.x+6,spriteStrech.position.y-40,Color(255,255,255));
  
  if(textureFilter == TyraLinear){ str_filter = "Filter: Linear";}
  else{ str_filter = "Filter: Nearest";}

  static int posX1 = sprite.position.x + 16;
  static int posX2 = spriteFlip.position.x + 16;
  static int posX3 = spriteStrech.position.x + 16;
  static int posX4 = spriteScale.position.x + 16;

  static int posY1 = sprite.position.y + (sprite.size.y * sprite.scale) + 16;
  static int posY2 = spriteFlip.position.y + (spriteFlip.size.y * spriteFlip.scale) + 16;
  static int posY3 = spriteStrech.position.y + (spriteStrech.size.y * spriteStrech.scale) + 16;
  static int posY4 = spriteScale.position.y + (spriteScale.size.y * spriteScale.scale) + 16;

  font.DrawText(str_filter,posX1,posY1,Color(255,255,255));
  font.DrawText("Filter: Linear",posX2,posY2,Color(255,255,255));
  font.DrawText(str_filter,posX3,posY3,Color(255,255,255));
  font.DrawText(str_filter,posX4,posY4,Color(255,255,255));

  /** End frame will perform vsync. */
  renderer.endFrame();
}

void Tutorial10::loadSprite() {
  const auto& screenSettings = engine->renderer.core.getSettings();

  int offset = 25;

  sprite.mode = SpriteMode::MODE_REPEAT;

  /** Let's scale it down */
  sprite.size = Vec2(32.0F, 32.0F);
  sprite.scale = 3;
  
  /** Set the position */
  sprite.position = Vec2(offset,screenSettings.getHeight() / 2.0F - sprite.size.y / 2.0F);

  spriteFlip.mode = SpriteMode::MODE_REPEAT;

  spriteFlip.flipHorizontal = true;
  spriteFlip.flipVertical = true;

  spriteFlip.size = Vec2(32.0F, 32.0F);

  spriteFlip.scale = 3;

  spriteFlip.position = Vec2(((offset*2) + (32.0F*3)),screenSettings.getHeight() / 2.0F - sprite.size.y / 2.0F);

  spriteScale.mode = SpriteMode::MODE_REPEAT;

  spriteScale.size = Vec2(96.0F,96.0F);

  spriteScale.position = Vec2(((offset*3) + ((32.0F*3)*2)),screenSettings.getHeight() / 2.0F - sprite.size.y / 2.0F);

  spriteStrech.mode = SpriteMode::MODE_STRETCH;

  spriteStrech.size = Vec2(96.0F,96.0F);

  spriteStrech.position = Vec2(((offset*4) + ((32.0F*3)*3)),screenSettings.getHeight() / 2.0F - sprite.size.y / 2.0F);

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
  texture->addLink(sprite.id);

  spriteFlip.id = sprite.id;
  spriteScale.id = sprite.id;
  spriteStrech.id = sprite.id;

  TYRA_LOG("Texture loaded!");
}

}  // namespace Tyra
