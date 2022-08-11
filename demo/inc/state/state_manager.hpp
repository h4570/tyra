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

#include <vector>
#include "./state.hpp"
#include "./global_state_type.hpp"

namespace Demo {

template <typename StateTypeT>
class StateManager {
 public:
  StateManager(const StateTypeT& t_initialState,
               const StateTypeT& t_exitState) {
    stateInitialized = false;
    currentState = t_initialState;
    exitState = t_exitState;
  }

  ~StateManager() { freeAll(); }

  const StateTypeT& getCurrentState() const { return currentState; }

  const StateTypeT& getExitState() const { return exitState; }

  bool finished() const { return currentState == exitState; }

  std::vector<State<StateTypeT>*>* getAll() { return &states; }

  State<StateTypeT>* get(const StateTypeT& stateType) {
    for (auto& state : states) {
      if (state->getState() == stateType) {
        return state;
      }
    }
    return nullptr;
  }

  void add(State<StateTypeT>* state) { states.push_back(state); }

  /** remove (without free) */
  void remove(const State<StateTypeT>* state) {
    states.erase(std::remove(states.begin(), states.end(), state),
                 states.end());
  }

  /** free and remove */
  void free(const State<StateTypeT>* state) {
    auto* found = get(state);
    if (!found) return;

    remove(found);
    delete found;
  }

  /** free and remove all */
  void freeAll() {
    for (auto& state : states) {
      delete state;
    }
    states.clear();
  }

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
  StateTypeT currentState, exitState;
};

}  // namespace Demo
