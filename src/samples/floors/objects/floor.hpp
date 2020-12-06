/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _FLOOR_
#define _FLOOR_

class Player; // Forward definition

#include <models/mesh.hpp>
#include <models/math/point.hpp>
#include <tamtypes.h>

/** Floor 3D object class  */
class Floor
{

public:
    u16 animTimer;
    s16 animDirection, initOffset, isByAudioTriggered;
    Mesh mesh;

    Floor();
    ~Floor();

    void init(const Mesh &mother, const Point &spiral, const u8 &initOffset);
    void animate(Player &player);
};

#endif
