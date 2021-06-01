/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "rocket_league.hpp"

int main()
{
    Engine engine = Engine();
    RocketLeague game = RocketLeague(&engine);
    game.engine->init(&game, 128);
    SleepThread();
    return 0;
}
