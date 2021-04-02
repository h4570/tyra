#include "cube.hpp"

int main()
{
    Engine engine = Engine();
    Cube game = Cube(&engine);
    game.engine->init(&game, 128);
    SleepThread();
    return 0;
}