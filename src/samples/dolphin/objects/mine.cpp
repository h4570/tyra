/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#include "mine.hpp"

u8 Mine::lastSoundBuffer = 4;

Mine::Mine()
{
    mesh.position.set(0.0F, 0.0F, 0.0F);

    bExploding = false;
    explosionTicks = 0;
}

Mine::~Mine()
{
}

void Mine::update()
{
    if (bExploding && explosionTicks < MINE_EXPLOSION_TICKS)
    {
        explosionTicks++;
        mesh.scale++;
    }
}
