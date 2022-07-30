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
#include "state/global_state_type.hpp"
#include "state/state_manager.hpp"
#include "./intro_state_type.hpp"

namespace Demo {

class IntroState : public State<GlobalStateType> {
 public:
  IntroState(Engine* t_engine);
  ~IntroState();

  const GlobalStateType& getState() const { return state; }

  const bool& wantFinish() const { return _wantFinish; };

  void onStart();

  void update();

  /** @return Next game state */
  GlobalStateType onFinish();

 private:
  GlobalStateType state;
  bool _wantFinish;

  StateManager<IntroStateType> stateManager;
};

}  // namespace Demo
