/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include "cubes.hpp"

int main()
{
    Engine engine = Engine();
    Cubes game = Cubes(&engine);
    game.engine->init(&game, 128);
    Tyra_Version version;
    version.Get();
    SleepThread();
    return 0;
}