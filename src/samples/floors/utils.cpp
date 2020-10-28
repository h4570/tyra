/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "./utils.hpp"

#include <fastmath.h>

/** Exponential ease-in-out animation 
 * @param t Time
 * @param b Start value
 * @param c Change
 * @param d Duration
 * @returns Current animation position
*/
float Utils::expoEaseInOut(float t, float b, float c, float d)
{
    if (t == 0)
        return b;

    if (t == d)
        return b + c;

    if ((t /= d / 2) < 1)
        return c / 2 * powf(2, 10 * (t - 1)) + b;

    return c / 2 * (-powf(2, -10 * --t) + 2) + b;
}
