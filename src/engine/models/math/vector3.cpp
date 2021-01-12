/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../../include/models/math/vector3.hpp"
#include "../../include/utils/math.hpp"

// ----
// Constructors/Destructors
// ----

Vector3::Vector3()
{
    x = 0.0F;
    y = 0.0F;
    z = 0.0F;
}

// ----
// Operators
// ----

Vector3 Vector3::operator+(const Vector3 &v) const
{
    Vector3 result;
    asm volatile( // VU0 Macro program
        "lqc2      $vf4, 0x0(%1)  \n\t"
        "lqc2      $vf5, 0x0(%2)  \n\t"
        "vadd.xyz  $vf6, $vf4, $vf5 \n\t"
        "sqc2      $vf6, 0x0(%0)  \n\t"
        :
        : "r"(result.xyz), "r"(this->xyz), "r"(v.xyz));
    return result;
}

Vector3 Vector3::operator-(const Vector3 &v) const
{
    Vector3 result;
    asm volatile( // VU0 Macro program
        "lqc2      $vf4, 0x0(%1)  \n\t"
        "lqc2      $vf5, 0x0(%2)  \n\t"
        "vsub.xyz  $vf6, $vf4, $vf5 \n\t"
        "sqc2      $vf6, 0x0(%0)  \n\t"
        :
        : "r"(result.xyz), "r"(this->xyz), "r"(v.xyz));
    return result;
}

Vector3 Vector3::operator*(const Vector3 &v) const
{
    Vector3 res;
    asm volatile(                                      // VU0 Macro program
        "lqc2           $vf4, 0x0(%1)            \n\t" // $vf4 = this
        "lqc2           $vf5, 0x0(%2)            \n\t" // $vf5 = v
        "vopmula.xyz    $ACC, $vf4,       $vf5     \n\t"
        "vopmsub.xyz    $vf8, $vf5,       $vf4     \n\t"
        "vsub.w         $vf8, $vf0,      $vf0    \n\t"
        "sqc2           $vf8, 0x0(%0)            \n\t" // $vf8 = res
        :
        : "r"(res.xyz), "r"(this->xyz), "r"(v.xyz));
    // result.x = y * v.z - z * v.y;
    // result.y = z * v.x - x * v.z;
    // result.z = x * v.y - y * v.x;
    return res;
}

Vector3 Vector3::operator*(const float &t) const
{
    Vector3 result;
    asm volatile(
        "lqc2       $vf4, 0x0(%1)  \n\t"
        "mfc1       $8,  %2       \n\t"
        "qmtc2      $8,  $vf5      \n\t"
        "vmulx.xyz  $vf6, $vf4, $vf5 \n\t"
        "sqc2       $vf6, 0x0(%0)  \n\t"
        :
        : "r"(result.xyz), "r"(this->xyz), "f"(t));
    return result;
}

Vector3 Vector3::operator/(const float &t) const
{
    Vector3 result;
    result.x = x / t;
    result.y = y / t;
    result.z = z / t;
    return result;
}

void Vector3::operator+=(const Vector3 &t)
{
    asm volatile( // VU0 Macro program
        "lqc2      $vf4, 0x0(%0)  \n\t"
        "lqc2      $vf5, 0x0(%1)  \n\t"
        "vadd.xyz  $vf4, $vf4, $vf5 \n\t"
        "sqc2      $vf4, 0x0(%0)  \n\t"
        :
        : "r"(this->xyz), "r"(t.xyz));
}

void Vector3::operator*=(const float &t)
{
    asm volatile(
        "lqc2       $vf4, 0x0(%0)  \n\t"
        "mfc1       $8,  %1       \n\t"
        "qmtc2      $8,  $vf5      \n\t"
        "vmulx.xyz  $vf4, $vf4, $vf5 \n\t"
        "sqc2       $vf4, 0x0(%0)  \n\t"
        :
        : "r"(this->xyz), "f"(t));
}

// ----
// Functions
// ----

void Vector3::set(const Vector3 &v)
{
    asm volatile( // VU0 Macro program
        "lq $6, 0x0(%1) \n\t"
        "sq $6, 0x0(%0) \n\t"
        :
        : "r"(this->xyz), "r"(v.xyz));
}

void Vector3::set(const float &t_x, const float &t_y, const float &t_z)
{
    x = t_x;
    y = t_y;
    z = t_z;
}

float Vector3::innerProduct(const Vector3 &v) const
{
    float result;
    asm volatile( // VU0 Macro program
        "lqc2     $vf4, 0x0(%1)  \n\t"
        "lqc2     $vf5, 0x0(%2)  \n\t"
        "vmul.xyz $vf6, $vf4, $vf5 \n\t"
        "vaddy.x  $vf6, $vf6, $vf6 \n\t"
        "vaddz.x  $vf6, $vf6, $vf6 \n\t"
        "qmfc2    $2,  $vf6      \n\t"
        "mtc1     $2,  %0       \n\t"
        : "=f"(result)
        : "r"(this->xyz), "r"(v.xyz));
    return result;
    // return (x * v.x + y * v.y + z * v.z);
}

float Vector3::length() const
{
    float result;
    asm volatile( // VU0 Macro program
        "lqc2     $vf4, 0x0(%1)  \n\t"
        "vmul.xyz $vf5, $vf4, $vf4 \n\t"
        "vaddy.x  $vf5, $vf5, $vf5 \n\t"
        "vaddz.x  $vf5, $vf5, $vf5 \n\t"
        "vsqrt    $Q  , $vf5x     \n\t"
        "vwaitq                 \n\t"
        "vaddq.x  $vf8, $vf0, $Q   \n\t"
        "qmfc2    $2,  $vf8      \n\t"
        "mtc1     $2,  %0       \n\t"
        : "=f"(result)
        : "r"(this->xyz));
    return result;
    // return Math::sqrt(x * x + y * y + z * z);
}

void Vector3::normalize()
{
    asm volatile( // VU0 Macro program
        "lqc2       $vf4, 0x0(%0)    \n\t"
        "vmul.xyz   $vf5, $vf4,  $vf4  \n\t"
        "vaddy.x    $vf5, $vf5,  $vf5  \n\t"
        "vaddz.x    $vf5, $vf5,  $vf5  \n\t"
        "vrsqrt     $Q,   $vf0w, $vf5x \n\t"
        "vwaitq                     \n\t"
        "vsub.xyz   $vf6, $vf0,  $vf0  \n\t"
        "vaddw.xyz  $vf6, $vf6,  $vf4  \n\t"
        "vwaitq                     \n\t"
        "vmulq.xyz  $vf6, $vf4,  $Q    \n\t"
        "sqc2       $vf6, 0x0(%0)    \n\t"
        :
        : "r"(this->xyz));
}

float Vector3::distanceTo(const Vector3 &v) const
{
    register float result;
    asm volatile( // VU0 Macro program
        "lqc2     $vf4, 0x0(%1)  \n\t"
        "lqc2     $vf5, 0x0(%2)  \n\t"
        "vsub.xyz $vf6, $vf4, $vf5 \n\t"
        "vmul.xyz $vf7, $vf6, $vf6 \n\t"
        "vaddy.x  $vf7, $vf7, $vf7 \n\t"
        "vaddz.x  $vf7, $vf7, $vf7 \n\t"
        "vsqrt    $Q  , $vf7x     \n\t"
        "vwaitq                 \n\t"
        "vaddq.x  $vf8, $vf0, $Q   \n\t"
        "qmfc2    $2,  $vf8      \n\t"
        "mtc1     $2,  %0       \n\t"
        : "=f"(result)
        : "r"(this->xyz), "r"(v.xyz));
    return result;
    // return Math::sqrt((this->x - v.x) * (this->x - v.x) +
    //                   (this->y - v.y) * (this->y - v.y) +
    //                   (this->z - v.z) * (this->z - v.z));
}

u8 Vector3::shouldBeBackfaceCulled(const Vector3 *t_cameraPos, const Vector3 *t_v0, const Vector3 *t_v1, const Vector3 *t_v2)
{
    register float dot;
    asm volatile(
        "lqc2        $vf4, 0x0(%1)  \n\t"   // $vf4 = cameraPos
        "lqc2        $vf5, 0x0(%2)  \n\t"   // $vf5 = v0
        "lqc2        $vf6, 0x0(%3)  \n\t"   // $vf6 = v1
        "lqc2        $vf7, 0x0(%4)  \n\t"   // $vf7 = v2
        "vsub.xyz    $vf8, $vf7, $vf5 \n\t" // $vf8 = $vf7(v2) - $vf5(v0)
        "vsub.xyz    $vf9, $vf6, $vf5 \n\t" // $vf9 = $vf6(v1) - $vf5(v0)
        "vopmula.xyz $ACC, $vf8, $vf9 \n\t" // $vf6 = cross($vf8, $vf9)
        "vopmsub.xyz $vf6, $vf9, $vf8 \n\t"
        "vsub.w      $vf6, $vf6, $vf6 \n\t"
        "vsub.xyz    $vf7, $vf5, $vf4 \n\t" // $vf7 = $vf5(v0) - $vf4(cameraPos)
        "vmul.xyz    $vf5, $vf7, $vf6 \n\t" // $vf5 = dot($vf7, $vf6)
        "vaddy.x     $vf5, $vf5, $vf5 \n\t"
        "vaddz.x     $vf5, $vf5, $vf5 \n\t"
        "qmfc2       $2,  $vf5      \n\t" // store result on `dot` variable
        "mtc1        $2,  %0       \n\t"
        : "=f"(dot)
        : "r"(t_cameraPos->xyz), "r"(t_v0->xyz), "r"(t_v1->xyz), "r"(t_v2->xyz));
    return dot <= 0.0F;
}

void Vector3::setByLerp(const Vector3 &t_v1, const Vector3 &t_v2, const float &t_interp, const float &t_scale)
{
    asm volatile(
        "lqc2      $vf4, 0x0(%1)  \n\t"   // $vf4 = v1
        "lqc2      $vf5, 0x0(%2)  \n\t"   // $vf5 = v2
        "mfc1      $8,  %3       \n\t"    // $vf6 = t
        "qmtc2     $8,  $vf6      \n\t"   // lerp:
        "vsub.xyz  $vf7, $vf5, $vf4 \n\t" // $vf7 = v2 - v1
        "vmulx.xyz $vf8, $vf7, $vf6 \n\t" // $vf8 = $vf7 * t
        "vadd.xyz  $vf9, $vf8, $vf4 \n\t" // $vf9 = $vf8 + $vf4
        "sqc2      $vf9, 0x0(%0)  \n\t"   // v0  = $vf9
        :
        : "r"(&this->xyz), "r"(&t_v1.xyz), "r"(&t_v2.xyz), "f"(t_interp));
    operator*=(t_scale);
}
