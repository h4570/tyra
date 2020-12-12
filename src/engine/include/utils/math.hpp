/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_MATH_
#define _TYRA_MATH_

#include <tamtypes.h>
#include <math3d.h>
#include "../models/math/vector3.hpp"

class Vector3; // Forward definition

void manyVec3ToNative(VECTOR *t_result, Vector3 *t_vec, int t_amount, float t_fourthVal);
void vec3ToNative(VECTOR t_result, const Vector3 &t_vec, float t_fourthVal);

class Math
{

public:
    const static float HALF_ANG2RAD = 3.14159265358979323846 / 360.0;
    const static float PI = 3.1415926535897932384626433832795F;
    const static float HALF_PI = 1.5707963267948966192313216916398F;
    static float cos(float x);
    static float asin(float x);
    static float mod(float x, float y);
    static inline float sin(float x) { return cos(x - HALF_PI); };
    static float sqrt(float x);
    static float invSqrt(float x);

private:
    Math();
};

#endif
