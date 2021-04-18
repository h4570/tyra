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
#include <fastmath.h>

class Vector3; // Forward definition

void manyVec3ToNative(VECTOR *t_result, Vector3 *t_vec, int t_amount, float t_fourthVal);
void vec3ToNative(VECTOR t_result, const Vector3 &t_vec, float t_fourthVal);

class Math
{

public:
    constexpr static float HALF_ANG2RAD = 3.14159265358979323846F / 360.0F;
    constexpr static float ANG2RAD = 3.14159265358979323846F / 180.0F;
    constexpr static float PI = 3.1415926535897932384626433832795F;
    constexpr static float HALF_PI = 1.5707963267948966192313216916398F;
    static float cos(float x);
    static float asin(float x);
    static float mod(float x, float y);
    static inline float sin(float x) { return cos(x - HALF_PI); };
    static inline float tan(float x) { return sin(x) / cos(x); }
    static float invSqrt(float x);
    static inline u32 max(u32 a, u32 b) { return (a > b) ? a : b; }
    static inline s32 max(s32 a, s32 b) { return (a > b) ? a : b; }
    static inline float max(float a, float b) { return (a > b) ? a : b; }
    static inline u32 min(u32 a, u32 b) { return (a < b) ? a : b; }
    static inline s32 min(s32 a, s32 b) { return (a < b) ? a : b; }
    static inline float min(float a, float b) { return (a < b) ? a : b; }

private:
    Math();
};

#endif
