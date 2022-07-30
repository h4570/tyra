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

#include <engine.hpp>

using Tyra::Engine;

namespace Demo {

template <typename StateTypeT>
class State {
 public:
  State(Engine* t_engine) : engine(t_engine) {}
  virtual ~State() {}

  virtual const StateTypeT& getState() const = 0;

  virtual const bool& wantFinish() const = 0;

  virtual void onStart() = 0;

  virtual void update() = 0;

  /** @return Next game state */
  virtual StateTypeT onFinish() = 0;

 protected:
  Engine* engine;
};

}  // namespace Demo
