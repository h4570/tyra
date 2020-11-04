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
#include <stdio.h>

// ----
// Constructors/Destructors
// ----

/** Create by specifying 3 points */
Vector3::Vector3(float t_x, float t_y, float t_z)
{
    x = t_x;
    y = t_y;
    z = t_z;
}

/** Create with another vector values */
Vector3::Vector3(const Vector3 &another)
{
    x = another.x;
    y = another.y;
    z = another.z;
}

void Vector3::setByLerp(const Vector3 &v1, const Vector3 &v2, const float &t_interp, const float &t_scale)
{
    asm volatile(
        "lqc2      vf4, 0x0(%1)  \n\t" // vf4 = v1
        "lqc2      vf5, 0x0(%2)  \n\t" // vf5 = v2
        "mfc1      $8,  %3       \n\t" // vf6 = t
        "qmtc2     $8,  vf6      \n\t" // lerp:
        "vsub.xyz  vf7, vf5, vf4 \n\t" // vf7 = v2 - v1
        "vmulx.xyz vf8, vf7, vf6 \n\t" // vf8 = vf7 * t
        "vadd.xyz  vf9, vf8, vf4 \n\t" // vf9 = vf8 + vf4
        "sqc2      vf9, 0x0(%0)  \n\t" // v0  = vf9
        :
        : "r"(&this->xyz), "r"(&v1.xyz), "r"(&v2.xyz), "f"(t_interp)
        : "$8");
    x *= t_scale;
    y *= t_scale;
    z *= t_scale;
}

/** Create empty vector */
Vector3::Vector3()
{
    x = 0;
    y = 0;
    z = 0;
}

Vector3::~Vector3() {}

// ----
// Methods
// ----

Vector3 Vector3::operator+(Vector3 v)
{
    Vector3 result;
    asm volatile( // VU0 Macro program
        "lqc2      vf4, 0x0(%1)  \n\t"
        "lqc2      vf5, 0x0(%2)  \n\t"
        "vadd.xyz  vf6, vf4, vf5 \n\t"
        "sqc2      vf6, 0x0(%0)  \n\t"
        :
        : "r"(result.xyz), "r"(this->xyz), "r"(v.xyz));
    return result;
}

Vector3 Vector3::operator-(const Vector3 &v)
{
    Vector3 result;
    asm volatile( // VU0 Macro program
        "lqc2      vf4, 0x0(%1)  \n\t"
        "lqc2      vf5, 0x0(%2)  \n\t"
        "vsub.xyz  vf6, vf4, vf5 \n\t"
        "sqc2      vf6, 0x0(%0)  \n\t"
        :
        : "r"(result.xyz), "r"(this->xyz), "r"(v.xyz));
    return result;
}

Vector3 Vector3::operator-(void)
{
    Vector3 result;
    result.x = -x;
    result.y = -y;
    result.z = -z;
    return result;
}

/** Also called "cross product" */
Vector3 Vector3::operator*(Vector3 &v)
{
    Vector3 res;
    asm volatile(                      // VU0 Macro program
        "lqc2      vf4, 0x0(%1)  \n\t" // vf4 = this
        "lqc2      vf5, 0x0(%2)  \n\t" // vf5 = v

        "vmulz.y   vf6, vf4, vf5 \n\t" // vf6.y = vf4.y * vf5.z
        "vmuly.z   vf6, vf4, vf5 \n\t"
        "vsubz.y   vf6, vf6, vf6 \n\t" // vf6.y = vf4.y - vf4.z
        "vaddy.x   vf8, vf0, vf6 \n\t" // res.x = vf4.y * vf5.z - vf4.z * vf5.y

        "vmulx.z   vf6, vf4, vf5 \n\t"
        "vmulz.x   vf6, vf4, vf5 \n\t"
        "vsubx.z   vf6, vf6, vf6 \n\t"
        "vaddz.y   vf8, vf0, vf6 \n\t" // res.y = vf4.z * vf5.x - vf4.x * vf5.z

        "vmuly.x   vf6, vf4, vf5 \n\t"
        "vmulx.y   vf6, vf4, vf5 \n\t"
        "vsuby.x   vf6, vf6, vf6 \n\t"
        "vaddx.z   vf8, vf0, vf6 \n\t" // res.z = vf4.x * vf5.y - vf4.y * vf5.x
        "sqc2      vf8, 0x0(%0)  \n\t"
        :
        : "r"(res.xyz), "r"(this->xyz), "r"(v.xyz));
    // result.x = y * v.z - z * v.y;
    // result.y = z * v.x - x * v.z;
    // result.z = x * v.y - y * v.x;
    return res;
}

Vector3 Vector3::operator*(float t)
{
    Vector3 result;
    asm volatile(
        "lqc2       vf4, 0x0(%1)  \n\t"
        "mfc1       $8,  %2       \n\t"
        "qmtc2      $8,  vf5      \n\t"
        "vmulx.xyz  vf6, vf4, vf5 \n\t"
        "sqc2       vf6, 0x0(%0)  \n\t"
        :
        : "r"(result.xyz), "r"(this->xyz), "f"(t)
        : "$8");
    return result;
}

Vector3 Vector3::operator/(float t)
{
    Vector3 result;
    result.x = x / t;
    result.y = y / t;
    result.z = z / t;
    return result;
}

u8 Vector3::shouldBeBackfaceCulled(const Vector3 *t_cameraPos, const Vector3 *v0, const Vector3 *v1, const Vector3 *v2)
{
    register float dot;
    asm volatile(
        "lqc2        vf4, 0x0(%1)  \n\t" // vf4 = cameraPos
        "lqc2        vf5, 0x0(%2)  \n\t" // vf5 = v0
        "lqc2        vf6, 0x0(%3)  \n\t" // vf6 = v1
        "lqc2        vf7, 0x0(%4)  \n\t" // vf7 = v2
        "vsub.xyz    vf8, vf7, vf5 \n\t" // vf8 = vf7(v2) - vf5(v0)
        "vsub.xyz    vf9, vf6, vf5 \n\t" // vf9 = vf6(v1) - vf5(v0)
        "vopmula.xyz ACC, vf8, vf9 \n\t" // vf6 = cross(vf8, vf9)
        "vopmsub.xyz vf6, vf9, vf8 \n\t"
        "vsub.w      vf6, vf6, vf6 \n\t"
        "vsub.xyz    vf7, vf5, vf4 \n\t" // vf7 = vf5(v0) - vf4(cameraPos)
        "vmul.xyz    vf5, vf7, vf6 \n\t" // vf5 = dot(vf7, vf6)
        "vaddy.x     vf5, vf5, vf5 \n\t"
        "vaddz.x     vf5, vf5, vf5 \n\t"
        "qmfc2       $2,  vf5      \n\t" // store result on `dot` variable
        "mtc1        $2,  %0       \n\t"
        : "=f"(dot)
        : "r"(t_cameraPos->xyz), "r"(v0->xyz), "r"(v1->xyz), "r"(v2->xyz)
        : "$2");
    return dot <= 0.0F;
}

/** Checks intersection with given square */
u8 Vector3::collidesSquare(Vector3 &t_min, Vector3 &t_max)
{
    return ((this->x <= t_max.x && this->x >= t_min.x) && (this->y < t_max.y && this->y >= t_min.y) && (this->z <= t_max.z && this->z >= t_min.z)) ? 1 : 0;
}

/** Checks is this vector is on given square */
u8 Vector3::isOnSquare(Vector3 &t_min, Vector3 &t_max)
{
    return ((this->x <= t_max.x && this->x >= t_min.x) && (this->y >= t_max.y) && (this->z <= t_max.z && this->z >= t_min.z)) ? 1 : 0;
}

float Vector3::length()
{
    register float result;
    asm volatile( // VU0 Macro program
        "lqc2     vf4, 0x0(%1)  \n\t"
        "vmul.xyz vf5, vf4, vf4 \n\t"
        "vaddy.x  vf5, vf5, vf5 \n\t"
        "vaddz.x  vf5, vf5, vf5 \n\t"
        "vsqrt    Q  , vf5x     \n\t"
        "vaddq.x  vf8, vf0, Q   \n\t"
        "qmfc2    $2,  vf8      \n\t"
        "mtc1     $2,  %0       \n\t"
        : "=f"(result)
        : "r"(this->xyz)
        : "$2");
    return result;
    // return Math::sqrt(x * x + y * y + z * z);
}

void Vector3::normalize()
{
    asm volatile( // VU0 Macro program
        "lqc2       vf4, 0x0(%0)    \n\t"
        "vmul.xyz   vf5, vf4,  vf4  \n\t"
        "vaddy.x    vf5, vf5,  vf5  \n\t"
        "vaddz.x    vf5, vf5,  vf5  \n\t"
        "vrsqrt     Q,   vf0w, vf5x \n\t"
        "vsub.xyz   vf6, vf0,  vf0  \n\t"
        "vaddw.xyz  vf6, vf6,  vf4  \n\t"
        "vwaitq                     \n\t"
        "vmulq.xyz  vf6, vf4,  Q    \n\t"
        "sqc2       vf6, 0x0(%0)    \n\t"
        :
        : "r"(this->xyz));
}

/** Also called dot3 */
float Vector3::innerProduct(Vector3 &v)
{
    register float result;
    asm volatile( // VU0 Macro program
        "lqc2     vf4, 0x0(%1)  \n\t"
        "lqc2     vf5, 0x0(%2)  \n\t"
        "vmul.xyz vf6, vf4, vf5 \n\t"
        "vaddy.x  vf6, vf6, vf6 \n\t"
        "vaddz.x  vf6, vf6, vf6 \n\t"
        "qmfc2    $2,  vf6      \n\t"
        "mtc1     $2,  %0       \n\t"
        : "=f"(result)
        : "r"(this->xyz), "r"(v.xyz)
        : "$2");
    return result;
    // return (x * v.x + y * v.y + z * v.z);
}

void Vector3::set(const float &x, const float &y, const float &z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector3::set(const Vector3 &v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

void Vector3::copy(Vector3 &v)
{
    asm volatile( // VU0 Macro program
        "lq $6, 0x0(%1) \n\t"
        "sq $6, 0x0(%0) \n\t"
        :
        : "r"(v.xyz), "r"(this->xyz)
        : "$6");
}

float Vector3::distanceTo(Vector3 &v)
{
    register float result;
    asm volatile( // VU0 Macro program
        "lqc2     vf4, 0x0(%1)  \n\t"
        "lqc2     vf5, 0x0(%2)  \n\t"
        "vsub.xyz vf6, vf4, vf5 \n\t"
        "vmul.xyz vf7, vf6, vf6 \n\t"
        "vaddy.x  vf7, vf7, vf7 \n\t"
        "vaddz.x  vf7, vf7, vf7 \n\t"
        "vsqrt    Q  , vf7x     \n\t"
        "vaddq.x  vf8, vf0, Q   \n\t"
        "qmfc2    $2,  vf8      \n\t"
        "mtc1     $2,  %0       \n\t"
        : "=f"(result)
        : "r"(this->xyz), "r"(v.xyz)
        : "$2");
    return result;
    // return Math::sqrt((this->x - another.x) * (this->x - another.x) +
    //                   (this->y - another.y) * (this->y - another.y) +
    //                   (this->z - another.z) * (this->z - another.z));
}

const void Vector3::print() const
{
    printf("Vector3(%f, %f, %f)\n", x, y, z);
}
