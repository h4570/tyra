/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _MOVABLE_
#define _MOVABLE_

#include <tamtypes.h>
#include <models/math/vector3.hpp>

class Movable
{

public:
    Movable(const float &preZRotation, const float &fullZRotation);
    ~Movable();

protected:
    u16 rotationPercentage;
    const u16 &updateRotationPercentage(const float &zRotation);
    void fixZRotation(Vector3 &rotation);

private:
    /** 
     * Calculate's REAL rotation percentage based on given Z rotation.
     * For example: -130%, 260%, 30%..
     */
    s16 getRealRotationPercentage(const float &zRotation) { return (zRotation - preZRotation) / (fullZRotation)*100; }
    float preZRotation;
    float fullZRotation;
};

#endif
