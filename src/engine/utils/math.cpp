/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/utils/math.hpp"

#include <math.h>
#include <fastmath.h>

float Math::cos(float x)
{
    float r;
    asm volatile(
        "lui     $9,  0x3f00        \n\t"
        ".set noreorder             \n\t"
        ".align 3                   \n\t"
        "abs.s   %0,  %1            \n\t"
        "lui     $8,  0xbe22        \n\t"
        "mtc1    $9,  $f1           \n\t"
        "ori     $8,  $8,    0xf983 \n\t"
        "mtc1    $8,  $f8           \n\t"
        "lui     $9,  0x4b00        \n\t"
        "mtc1    $9,  $f3           \n\t"
        "lui     $8,  0x3f80        \n\t"
        "mtc1    $8,  $f2           \n\t"
        "mula.s  %0,  $f8           \n\t"
        "msuba.s $f3, $f2           \n\t"
        "madda.s $f3, $f2           \n\t"
        "lui     $8,  0x40c9        \n\t"
        "msuba.s %0,  $f8           \n\t"
        "ori     $8,  0x0fdb        \n\t"
        "msub.s  %0,  $f1,   $f2    \n\t"
        "lui     $9,  0xc225        \n\t"
        "abs.s   %0,  %0            \n\t"
        "lui     $10, 0x3e80        \n\t"
        "mtc1    $10, $f7           \n\t"
        "ori     $9,  0x5de1        \n\t"
        "sub.s   %0,  %0,    $f7    \n\t"
        "lui     $10, 0x42a3        \n\t"
        "mtc1    $8,  $f3           \n\t"
        "ori     $10, 0x3458        \n\t"
        "mtc1    $9,  $f4           \n\t"
        "lui     $8,  0xc299        \n\t"
        "mtc1    $10, $f5           \n\t"
        "ori     $8,  0x2663        \n\t"
        "mul.s   $f8, %0,    %0     \n\t"
        "lui     $9,  0x421e        \n\t"
        "mtc1    $8,  $f6           \n\t"
        "ori     $9,  0xd7bb        \n\t"
        "mtc1    $9,  $f7           \n\t"
        "nop                        \n\t"
        "mul.s   $f1, %0,    $f8    \n\t"
        "mul.s   $f9, $f8,   $f8    \n\t"
        "mula.s  $f3, %0            \n\t"
        "mul.s   $f2, $f1,   $f8    \n\t"
        "madda.s $f4, $f1           \n\t"
        "mul.s   $f1, $f1,   $f9    \n\t"
        "mul.s   %0,  $f2,   $f9    \n\t"
        "madda.s $f5, $f2           \n\t"
        "madda.s $f6, $f1           \n\t"
        "madd.s  %0,  $f7,   %0     \n\t"
        ".set reorder               \n\t"
        : "=&f"(r)
        : "f"(x)
        : "$f1", "$f2", "$f3", "$f4", "$f5", "$f6", "$f7", "$f8", "$f9", "$8", "$9", "$10");
    return r;
}

float Math::sqrt(float x)
{
    float r;
    __asm__ volatile(
        "sqrt.s %0, %1 \n\t"
        : "=&f"(r)
        : "f"(x));
    return r;
}

float Math::invSqrt(float x) { return 1.0F / sqrt(x); }

/** Converts Vector3 to PS2SDK's Vector4 */
void vec3ToNative(VECTOR o_result, const Vector3 &t_vec, float t_fourthVal)
{
    o_result[0] = t_vec.x;
    o_result[1] = t_vec.y;
    o_result[2] = t_vec.z;
    o_result[3] = t_fourthVal;
}

/** Converts array of Vector3 to array of PS2SDK's Vector4 */
void manyVec3ToNative(VECTOR *o_result, Vector3 *t_vec, int t_amount, float t_fourthVal)
{
    for (int i = 0; i < t_amount; i++)
        vec3ToNative(o_result[i], t_vec[i], t_fourthVal);
}
