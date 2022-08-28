#include <tyra>
#include "racer_game.hpp"

int main() {
  Tyra::Engine engine;
  Racer::RacerGame game(&engine);
  engine.run(&game);
  SleepThread();
  return 0;
}
