/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/intro/states/intro_war_info_state.hpp"
#include "loaders/texture/png_loader.hpp"
#include "file/file_utils.hpp"
#include "debug/debug.hpp"
#include "thread/threading.hpp"
#include "math/math.hpp"

using Tyra::FileUtils;
using Tyra::Math;
using Tyra::PngLoader;
using Tyra::Threading;

namespace Demo {

IntroWarInfoState::IntroWarInfoState(Engine* t_engine) : State(t_engine) {
  state = STATE_WAR_INFO;
  _wantFinish = false;
}

IntroWarInfoState::~IntroWarInfoState() {
  if (!initialized) return;
}

void IntroWarInfoState::onStart() {
  warInfoSprite = new Sprite;
  warInfoSprite->size.set(512.0F, 512.0F);
  warInfoSprite->position.set(0.0F, 0.0F);

  warInfoTexture = engine->renderer.core.texture.repository.add(
      FileUtils::fromCwd("intro/war_info.png"));
  warInfoTexture->addLink(warInfoSprite->getId());

  initialized = true;
}

void IntroWarInfoState::update() {
  engine->renderer.beginFrame();

  Threading::switchThread();

  engine->renderer.renderer2D.render(warInfoSprite);

  engine->renderer.endFrame();
}
IntroStateType IntroWarInfoState::onFinish() { return STATE_PRESS_KEY; }

}  // namespace Demo
