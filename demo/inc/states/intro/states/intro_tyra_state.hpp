/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "state/state.hpp"
#include "../intro_state_type.hpp"
#include "renderer/core/2d/sprite/sprite.hpp"
#include "time/timer.hpp"

using Tyra::Sprite;
using Tyra::Texture;
using Tyra::Timer;

namespace Demo {

class IntroTyraState : public State<IntroStateType> {
 public:
  IntroTyraState(Engine* t_engine);
  ~IntroTyraState();

  const IntroStateType& getState() const { return state; }

  const bool& wantFinish() const { return _wantFinish; };

  void onStart();

  void update();

  /** @return Next game state */
  IntroStateType onFinish();

 private:
  IntroStateType state;
  bool _wantFinish;

  Texture* tyraTexture;
  Texture* bgTexture;
  Texture* bg2Texture;
  Texture* fillerTexture;

  Timer initialDelayTimer;
  bool initialized;
  bool initialDelayElapsed;
  bool animActivated;
  bool fadeoutActivated;
  u8 frameSkipper;

  Sprite* tyraSprite;
  Sprite* bgSprite;
  Sprite* bg2Sprite;
  Sprite* fillerSprite;

  void renderFillers();
};

}  // namespace Demo
