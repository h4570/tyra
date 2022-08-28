#pragma once

#include <tyra>

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
