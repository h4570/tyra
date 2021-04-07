#include "cubes.hpp"

int main()
{
    PRINT_LOG("INIT");
    Engine engine = Engine();
    Cubes game = Cubes(&engine);
    game.engine->init(&game, 128);
    SleepThread();
    return 0;
}