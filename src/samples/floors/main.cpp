/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "floors.hpp"

int main()
{
    Engine engine = Engine();
    Floors game = Floors(&engine);
    game.engine->init(&game, 128);
    SleepThread();
    return 0;
}
