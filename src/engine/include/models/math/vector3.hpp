/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_VECTOR3_
#define _TYRA_VECTOR3_

#include <tamtypes.h>

class Math; // Forward definition

/** https://en.wikipedia.org/wiki/Vector_(mathematics_and_physics) */
class Vector3
{

public:
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        float xyz[3] __attribute__((__aligned__(16)));
    };

    Vector3(float t_x, float t_y, float t_z);
    Vector3(const Vector3 &v);
    Vector3();
    ~Vector3();

    Vector3 operator+(Vector3 v);
    Vector3 operator-(const Vector3 &v);
    Vector3 operator*(Vector3 &v);
    Vector3 operator*(const float &t);
    void operator*=(const float &t);
    Vector3 operator/(float t);
    Vector3 operator-(void);

    static u8 shouldBeBackfaceCulled(const Vector3 *t_cameraPos, const Vector3 *v0, const Vector3 *v1, const Vector3 *v2);
    u8 collidesSquare(const Vector3 &t_min, const Vector3 &t_max) const;
    u8 isOnSquare(const Vector3 &t_min, const Vector3 &t_max) const;
    float length();
    void normalize();
    void setByLerp(const Vector3 &v1, const Vector3 &v2, const float &t_interp, const float &t_scale);
    float innerProduct(Vector3 &v);
    void set(const Vector3 &v);
    void rotate(const Vector3 &v, u8 inversed = false);
    void set(const float &t_x, const float &t_y, const float &t_z);
    void copy(Vector3 &v);
    float distanceTo(const Vector3 &v) const;
    const void print() const;
};

#endif
