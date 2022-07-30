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

#include <vector>
#include "./state.hpp"
#include "./global_state_type.hpp"

namespace Demo {

template <typename StateTypeT>
class StateManager {
 public:
  StateManager(const StateTypeT& initialState) {
    stateInitialized = false;
    currentState = initialState;
  }

  ~StateManager() {
    for (auto& state : states) {
      delete state;
    }
  }

  const StateTypeT& getCurrentState() const { return currentState; }

  void add(State<StateTypeT>* state) { states.push_back(state); }

  void update() {
    for (auto& state : states) {
      if (state->getState() == currentState) {
        if (state->wantFinish()) {
          currentState = state->onFinish();
          stateInitialized = false;
          break;
        }

        if (!stateInitialized) {
          state->onStart();
          stateInitialized = true;
        }

        state->update();
        break;
      }
    }
  }

 private:
  bool stateInitialized;
  std::vector<State<StateTypeT>*> states;
  StateTypeT currentState;
};

}  // namespace Demo
