/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/intro/states/intro_tyra_state.hpp"

using Tyra::FileUtils;
using Tyra::Math;
using Tyra::PngLoader;
using Tyra::Threading;

namespace Demo {

IntroTyraState::IntroTyraState(Engine* t_engine) : State(t_engine) {
  state = STATE_TYRA;
  _wantFinish = false;
  initialized = false;
  initialDelayElapsed = false;
  animActivated = true;
  fadeoutActivated = false;
  frameSkipper = 0;
}

IntroTyraState::~IntroTyraState() {}

void IntroTyraState::onStart() {
  TYRA_LOG("Intro - tyra. RAM: ", engine->info.getAvailableRAM(), "MB");

  const auto& settings = engine->renderer.core.getSettings();

  initialDelayTimer.prime();

  tyraSprite = new Sprite;
  tyraSprite->size.set(256.0F, 64.0F);
  tyraSprite->position.set(settings.getWidth() / 2 - tyraSprite->size.x / 2,
                           settings.getHeight() / 2 - tyraSprite->size.y / 2);

  bgSprite = new Sprite;
  bgSprite->size.set(256.0F, 256.0F);
  bgSprite->position.set(settings.getWidth() / 2 - bgSprite->size.x / 2,
                         settings.getHeight() / 3 * 2.0F);

  bg2Sprite = new Sprite;
  bg2Sprite->size.set(256.0F, 256.0F);
  bg2Sprite->position.set(
      settings.getWidth() / 2 - bg2Sprite->size.x / 2,
      (settings.getHeight() / 3 * 2.0F) + bg2Sprite->size.y + 1.0F);

  fillerSprite = new Sprite;
  fillerSprite->size.set(256.0F, 256.0F);
  fillerSprite->position.set(0.0F, 0.0F);

  tyraTexture = engine->renderer.core.texture.repository.add(
      FileUtils::fromCwd("intro/tyra.png"));
  tyraTexture->addLink(tyraSprite->id);

  bgTexture = engine->renderer.core.texture.repository.add(
      FileUtils::fromCwd("intro/tyra_bg.png"));
  bgTexture->addLink(bgSprite->id);

  Threading::switchThread();

  bg2Texture = engine->renderer.core.texture.repository.add(
      FileUtils::fromCwd("intro/tyra_bg2.png"));
  bg2Texture->addLink(bg2Sprite->id);

  fillerTexture = engine->renderer.core.texture.repository.add(
      FileUtils::fromCwd("intro/filler.png"));
  fillerTexture->addLink(fillerSprite->id);

  initialized = true;
}

IntroStateType IntroTyraState::onFinish() {
  if (!initialized) return STATE_PRESS_KEY;

  engine->renderer.core.texture.repository.free(tyraTexture->id);
  engine->renderer.core.texture.repository.free(bgTexture->id);
  engine->renderer.core.texture.repository.free(bg2Texture->id);
  engine->renderer.core.texture.repository.free(fillerTexture->id);

  delete tyraSprite;
  delete bgSprite;
  delete bg2Sprite;
  delete fillerSprite;

  initialized = false;
  return STATE_PRESS_KEY;
}

void IntroTyraState::update() {
  engine->renderer.beginFrame();

  if (initialDelayTimer.getTimeDelta() >= 15000) initialDelayElapsed = true;

  frameSkipper++;
  if (frameSkipper > 3) {
    frameSkipper = 0;
  }

  if (engine->pad.getClicked().Cross) {
    _wantFinish = true;
  }

  const float slideUpMaxPost = -128.0F;

  if (initialDelayElapsed) {
    if (bgSprite->position.y <= 32.0F) fadeoutActivated = true;

    float offset = 1.0F;
    if (bgSprite->position.y > slideUpMaxPost && bgSprite->position.y <= 64.0F)
      offset /= 1.6F;
    else if (bgSprite->position.y > 64.0F && bgSprite->position.y <= 128.0F)
      offset /= 1.2F;

    if (bgSprite->position.y > 32.0F && bgSprite->position.y <= 64.0F &&
        bgSprite->color.g > 0.0F) {
      const float colorOffset = 1.5F;
      bgSprite->color.g -= colorOffset;
      bgSprite->color.b -= colorOffset;
      bg2Sprite->color.g -= colorOffset;
      bg2Sprite->color.b -= colorOffset;
    }

    if (bgSprite->position.y > slideUpMaxPost) {
      bgSprite->position.y -= offset;
      bg2Sprite->position.y -= offset;
    }
  }

  if (fadeoutActivated && frameSkipper == 0) {
    if (bgSprite->color.a > 0) {
      bgSprite->color.a -= 4.0F;
      bg2Sprite->color.a -= 4.0F;
    } else
      _wantFinish = true;
  }

  Threading::switchThread();

  engine->renderer.renderer2D.render(bgSprite);
  engine->renderer.renderer2D.render(bg2Sprite);
  engine->renderer.renderer2D.render(tyraSprite);

  renderFillers();

  engine->renderer.endFrame();
}

void IntroTyraState::renderFillers() {
  // Up
  fillerSprite->position.set(0.0F, -63.0F);
  engine->renderer.renderer2D.render(fillerSprite);

  fillerSprite->position.set(256.0F, -63.0F);
  engine->renderer.renderer2D.render(fillerSprite);

  // Left
  fillerSprite->position.set(-124.0F, 128.0F);
  engine->renderer.renderer2D.render(fillerSprite);

  // Down
  fillerSprite->position.set(0.0F, 256.0F);
  engine->renderer.renderer2D.render(fillerSprite);

  fillerSprite->position.set(256.0F, 256.0F);
  engine->renderer.renderer2D.render(fillerSprite);

  // Right
  fillerSprite->position.set(386.0F, 128.0F);
  engine->renderer.renderer2D.render(fillerSprite);
}

}  // namespace Demo
