/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "movable.hpp"
#include <utils/debug.hpp>

// ----
// Constructors/Destructors
// ----

Movable::Movable(const float &preZRotation, const float &fullZRotation)
    : rotationPercentage(0), preZRotation(preZRotation), fullZRotation(fullZRotation) {}

Movable::~Movable() {}

// ----
// Methods
// ----

/** 
 * Calculates REAL rotation percentage based on given Z rotation (ex. -130%)
 * and convert's it to 0-100 percentage (ex -30% -> 70%)
 * NOTICE: REAL rotation percentage must be in range -100 - 100, so fixZRotation() may be needed!
 */
const u16 &Movable::updateRotationPercentage(const float &zRotation)
{
    const s16 newRotationPerc = getRealRotationPercentage(zRotation);
    assertMsg(newRotationPerc <= 100 && newRotationPerc >= -100, "Wrong range in new rotation percentage!");
    if (newRotationPerc < 0)
        rotationPercentage = 100 + newRotationPerc;
    else
        rotationPercentage = newRotationPerc;
    return rotationPercentage;
}

/** 
 * Calculate's REAL rotation percentage based on given Z rotation (ex. -130%)
 * and fixes given Z rotation if it turned by more that -+100% (ex -130% -> -30%)
 */
void Movable::fixZRotation(Vector3 &rotation)
{
    const s16 newRotationPerc = getRealRotationPercentage(rotation.z);
    if (newRotationPerc > 100)
        rotation.z -= fullZRotation;
    else if (newRotationPerc < -100)
        rotation.z += fullZRotation;
}