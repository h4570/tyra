/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/intro/states/intro_press_key_state.hpp"
#include <string>

using Tyra::FileUtils;
using Tyra::Math;
using Tyra::PngLoader;
using Tyra::SpriteMode;
using Tyra::Threading;

namespace Demo {

IntroPressKeyState::IntroPressKeyState(Engine* t_engine)
    : State(t_engine), mapPosition(0.0F, 0.0F) {
  state = STATE_PRESS_KEY;
  _wantFinish = false;
  mapDirection = 0;
  mapFadeIn = true;
  fillerFadeIn = false;
  showPressKey = false;
  pressKeyAlphaToggle = false;
  pressKeyAlphaDelayer = 0;
  fillersOffset = -512.0F;
}

IntroPressKeyState::~IntroPressKeyState() {}

void IntroPressKeyState::onStart() {
  TYRA_LOG("Intro - presskey. RAM: ", engine->info.getAvailableRAM(), "MB");

  for (u8 i = 0; i < mapRows; i++) {
    for (u8 j = 0; j < mapCols; j++) {
      mapSprites[i][j] = new Sprite;
      mapSprites[i][j]->color.a = 0.0F;
      mapSprites[i][j]->setMode(SpriteMode::MODE_STRETCH);
      mapSprites[i][j]->size.set(textureWidthHeight, textureWidthHeight);
      mapSprites[i][j]->position.set(i * textureWidthHeight,
                                     j * textureWidthHeight);

      std::string filename =
          "map-" + std::to_string(i) + "-" + std::to_string(j);
      std::string path = "intro/map/" + filename + ".png";

      mapTextures[i][j] = engine->renderer.core.texture.repository.add(
          FileUtils::fromCwd(path));
      mapTextures[i][j]->addLink(mapSprites[i][j]->id);
    }
  }

  fillerSprite = new Sprite;
  fillerSprite->size.set(256.0F, 50.0F);
  fillerSprite->position.set(0.0F, 224.0F - 25.0F);

  fillerTexture = engine->renderer.core.texture.repository.add(
      FileUtils::fromCwd("intro/filler.png"));
  fillerTexture->addLink(fillerSprite->id);

  pressKeySprite = new Sprite;
  pressKeySprite->size.set(256.0F, 64.0F);
  pressKeySprite->scale = 0.5F;
  pressKeySprite->position.set(192.0F, 380.0F);

  pressKeyTexture = engine->renderer.core.texture.repository.add(
      FileUtils::fromCwd("intro/press-key.png"));
  pressKeyTexture->addLink(pressKeySprite->id);

  logoSprite = new Sprite;
  logoSprite->size.set(256.0F, 128.0F);
  logoSprite->position.set(256.0F - 128.0F, 224.0F - 64.0F);

  logoTexture = engine->renderer.core.texture.repository.add(
      FileUtils::fromCwd("intro/lastalive.png"));
  logoTexture->addLink(logoSprite->id);

  initialized = true;
}

IntroStateType IntroPressKeyState::onFinish() {
  if (!initialized) return STATE_INTRO_END;

  for (u8 i = 0; i < mapRows; i++)
    for (u8 j = 0; j < mapCols; j++) {
      engine->renderer.core.texture.repository.free(mapTextures[i][j]->id);
      delete mapSprites[i][j];
    }

  engine->renderer.core.texture.repository.free(fillerTexture->id);
  delete fillerSprite;

  engine->renderer.core.texture.repository.free(logoTexture->id);
  delete logoSprite;

  engine->renderer.core.texture.repository.free(pressKeyTexture->id);
  delete pressKeySprite;

  initialized = false;
  return STATE_INTRO_END;
}

void IntroPressKeyState::update() {
  engine->renderer.beginFrame();

  if (mapDirection == 0 &&
      mapPosition.x >= textureWidthHeight * (mapRows - 1) &&
      mapPosition.y >= textureWidthHeight * (mapCols - 1)) {
    mapDirection = 1;
  } else if (mapDirection == 1 && mapPosition.x <= 0.0F &&
             mapPosition.y <= 0.0F) {
    mapDirection = 0;
  }

  if (engine->pad.getClicked().Cross) {
    _wantFinish = true;
  }

  const float offset = 0.1F;
  if (mapDirection == 0) {
    mapPosition += offset;
  } else if (mapDirection == 1) {
    mapPosition -= offset;
  }

  if (mapFadeIn) {
    for (u8 i = 0; i < mapRows; i++) {
      for (u8 j = 0; j < mapCols; j++) {
        auto* sprite = mapSprites[i][j];
        if (sprite->color.a >= 16.0F && sprite->color.a < 32.0F) {
          fillerFadeIn = true;
        }
        if (sprite->color.a < 96.0F) {
          sprite->color.a += 0.2F;
        } else {
          mapFadeIn = false;
        }
      }
    }
  }

  if (fillerFadeIn) {
    if (fillersOffset < -1.0F) {
      fillersOffset += 4.0F;
    } else {
      fillerFadeIn = false;
      showPressKey = true;
    }
  }

  updateMap();
  Threading::switchThread();

  for (u8 i = 0; i < mapRows; i++)
    for (u8 j = 0; j < mapCols; j++)
      engine->renderer.renderer2D.render(mapSprites[i][j]);

  renderFiller();
  engine->renderer.renderer2D.render(logoSprite);

  if (showPressKey) {
    if (pressKeyAlphaDelayer++ > 50) {
      pressKeyAlphaToggle = !pressKeyAlphaToggle;
      pressKeyAlphaDelayer = 0;
    }

    if (pressKeyAlphaToggle) {
      engine->renderer.renderer2D.render(pressKeySprite);
    }
  }

  engine->renderer.endFrame();
}

void IntroPressKeyState::renderFiller() {
  fillerSprite->position.x = -1.0F + fillersOffset;
  engine->renderer.renderer2D.render(fillerSprite);
  fillerSprite->position.x = 255.0F + fillersOffset;
  engine->renderer.renderer2D.render(fillerSprite);
}

void IntroPressKeyState::updateMap() {
  for (u8 i = 0; i < mapRows; i++)
    for (u8 j = 0; j < mapCols; j++) {
      mapSprites[i][j]->position.x =
          i * (textureWidthHeight + 1.0F) - mapPosition.x;
      mapSprites[i][j]->position.y =
          j * (textureWidthHeight + 1.0F) - mapPosition.y;
    }
}
}  // namespace Demo
