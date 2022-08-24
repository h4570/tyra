#include "racer_game.hpp"

namespace Racer {

using namespace Tyra;

RacerGame::RacerGame(Engine* t_engine) { engine = t_engine; }
RacerGame::~RacerGame() {}

void RacerGame::init() { TYRA_LOG("Hello!"); }

void RacerGame::loop() { TYRA_LOG("Loop!"); }

}  // namespace Racer
