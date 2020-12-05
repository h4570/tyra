/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#ifndef _COLLECTIBLE_
#define _COLLECTIBLE_

#include "../camera.hpp"
#include <tamtypes.h>

class Collectible
{

public:
    float rotation;
    Mesh mesh;
    Collectible();
    ~Collectible();
    void update();
    void setActive(u8 b);
    u8 isActive();

private:
    u8 bActive;
};

#endif
