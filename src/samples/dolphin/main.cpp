/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#include "dolphin.hpp"

int main()
{
    Engine engine = Engine();
    Dolphin game = Dolphin(&engine);
    game.engine->init(&game, 128);
    SleepThread();
    return 0;
}