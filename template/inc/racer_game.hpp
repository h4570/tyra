#pragma once

#include <engine.hpp>
#include <game.hpp>

namespace Racer {

class RacerGame : public Tyra::Game {
 public:
  RacerGame(Tyra::Engine* engine);
  ~RacerGame();

  void init();
  void loop();

 private:
  Tyra::Engine* engine;
};

}  // namespace Racer
