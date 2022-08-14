/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/intro/states/intro_ps2dev_state.hpp"

using Tyra::FileUtils;
using Tyra::PngLoader;
using Tyra::Threading;

namespace Demo {

IntroPs2DevState::IntroPs2DevState(Engine* t_engine) : State(t_engine) {
  state = STATE_PS2DEV;
  _wantFinish = false;
  initialized = false;
  initialDelayElapsed = false;
  frameSkipper = 0;
  fadeinActivated = true;
  fadeoutActivated = false;
}

IntroPs2DevState::~IntroPs2DevState() {}

void IntroPs2DevState::onStart() {
  TYRA_LOG("Intro - ps2dev. RAM: ", engine->info.getAvailableRAM(), "MB");

  const auto& settings = engine->renderer.core.getSettings();

  initialDelayTimer.prime();

  sprite = new Sprite;
  sprite->size.set(256.0F, 64.0F);
  sprite->position.set(settings.getWidth() / 2 - sprite->size.x / 2,
                       settings.getHeight() / 2 - sprite->size.y / 2);
  sprite->color.a = 0;

  texture = engine->renderer.core.texture.repository.add(
      FileUtils::fromCwd("intro/ps2dev.png"));
  texture->addLink(sprite->id);

  initialized = true;
}

IntroStateType IntroPs2DevState::onFinish() {
  if (!initialized) return STATE_TYRA;

  delete sprite;
  engine->renderer.core.texture.repository.free(texture->id);

  initialized = false;
  return STATE_TYRA;
}

void IntroPs2DevState::update() {
  engine->renderer.beginFrame();

  if (initialDelayTimer.getTimeDelta() >= 60000) initialDelayElapsed = true;

  if (engine->pad.getClicked().Cross) {
    _wantFinish = true;
  }

  frameSkipper++;
  if (frameSkipper > 3) {
    frameSkipper = 0;
  }

  if (fadeinActivated && initialDelayElapsed && frameSkipper == 0) {
    if (sprite->color.a < 128.0F)
      sprite->color.a += 2.0F;
    else {
      fadeoutActivated = true;
      fadeinActivated = false;
    }
    if (sprite->color.a >= 96.0F && sprite->color.a < 128.0F) {
      sprite->color.g -= 6.0F;
      sprite->color.b -= 6.0F;
    }
  }

  Threading::switchThread();

  if (fadeoutActivated && frameSkipper == 0) {
    if (sprite->color.a > 0)
      sprite->color.a -= 4;
    else
      _wantFinish = true;
  }

  engine->renderer.renderer2D.render(sprite);

  engine->renderer.endFrame();
}

}  // namespace Demo
