/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#ifndef _MINE_
#define _MINE_

#include <tamtypes.h>
#include <models/mesh.hpp>

#define MINE_EXPLOSION_TICKS 10

class Mine
{
public:
    static u8 lastSoundBuffer;
    Mesh mesh;
    void explode() { bExploding = true; }
    void update();
    Mine();
    ~Mine();
    const u8 &getExplosionTicks() { return explosionTicks; }

private:
    u8 bExploding;
    u8 explosionTicks;
};

#endif
