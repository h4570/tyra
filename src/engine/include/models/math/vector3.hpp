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
#include <stdio.h>

class Math; // Forward definition

/** https://en.wikipedia.org/wiki/Vector_(mathematics_and_physics) */
class Vector3
{

public:
    /** This trick will allow use to use vec.x and vex[0] */
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

    // ----
    // Constructors/Destructors
    // ----

    /** Create empty vector. */
    Vector3();

    /** Create vector with given vector values. */
    Vector3(const Vector3 &t_v) { set(t_v); }

    /** Create vector with given values. */
    Vector3(const float &t_x, const float &t_y, const float &t_z) { set(t_x, t_y, t_z); }

    // ----
    // Operators
    // ----

    Vector3 operator+(const Vector3 &v) const;
    Vector3 operator-(const Vector3 &v) const;
    /** Also called "cross product" */
    Vector3 operator*(const Vector3 &v) const;
    Vector3 operator*(const float &t) const;
    Vector3 operator/(const float &t) const;
    Vector3 operator-(void) const { return Vector3(-x, -y, -z); }
    void operator+=(const Vector3 &t);
    void operator*=(const float &t);

    // ----
    // Functions
    // ----

    /** Set vector values via VU0. */
    void set(const Vector3 &v);

    /** Set vector values. */
    void set(const float &t_x, const float &t_y, const float &t_z);

    /** Also called dot3. */
    float innerProduct(const Vector3 &v) const;

    /** Get vector length */
    float length() const;

    /** Normalize vector. */
    void normalize();

    /** Returns distance between two vectors */
    float distanceTo(const Vector3 &v) const;

    /** 
     * Checks intersection with given box
     * @param t_min Opposite min vertex (ex. near, left, down vertex of bounding box)
     * @param t_max Opposite max vertex (ex. far, right, up vertex of bounding box)
     */
    u8 collidesBox(const Vector3 &t_min, const Vector3 &t_max) const
    {
        return ((this->x <= t_max.x && this->x >= t_min.x) && (this->y < t_max.y && this->y >= t_min.y) && (this->z <= t_max.z && this->z >= t_min.z)) ? 1 : 0;
    }

    /** 
     * Checks if this vector is on given box (this->y >= box.y)
     * @param t_min Opposite min vertex (ex. near, left, down vertex of bounding box)
     * @param t_max Opposite max vertex (ex. far, right, up vertex of bounding box)
     */
    u8 isOnBox(const Vector3 &t_min, const Vector3 &t_max) const
    {
        return ((this->x <= t_max.x && this->x >= t_min.x) && (this->y >= t_max.y) && (this->z <= t_max.z && this->z >= t_min.z)) ? 1 : 0;
    }

    /** Check if given triangle should be backface culled. */
    static u8 shouldBeBackfaceCulled(const Vector3 *t_cameraPos, const Vector3 *t_v0, const Vector3 *t_v1, const Vector3 *t_v2);

    /** 
     * Set by linear interpolation between two vertices (animation). 
     * @param t_v1 Before vertex 
     * @param t_v2 After vertex 
     * @param t_interp State of interpolation 
     * @param t_scale Scale 
     */
    void setByLerp(const Vector3 &t_v1, const Vector3 &t_v2, const float &t_interp, const float &t_scale);

    const void print() const
    {
        printf("Vector3(%f, %f, %f)\n", x, y, z);
    }
};

#endif
