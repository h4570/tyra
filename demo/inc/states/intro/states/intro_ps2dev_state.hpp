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

#include "state/state.hpp"
#include "../intro_state_type.hpp"
#include <tyra>

using Tyra::Sprite;
using Tyra::Texture;
using Tyra::Timer;

namespace Demo {

class IntroPs2DevState : public State<IntroStateType> {
 public:
  IntroPs2DevState(Engine* t_engine);
  ~IntroPs2DevState();

  const IntroStateType& getState() const { return state; }

  const bool& wantFinish() const { return _wantFinish; };

  void onStart();

  void update();

  /** @return Next game state */
  IntroStateType onFinish();

 private:
  IntroStateType state;
  bool _wantFinish;

  Texture* texture;
  Timer initialDelayTimer;
  bool initialized;
  bool initialDelayElapsed;
  bool fadeinActivated;
  bool fadeoutActivated;
  u8 frameSkipper;

  Sprite* sprite;
};

}  // namespace Demo
