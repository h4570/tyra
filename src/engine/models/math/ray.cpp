/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Welligton Carvalho <wellcoj@gmail.com>
*/

#include "../include/models/math/ray.hpp"

// ----
// Constructors/Destructors
// ----

Ray::Ray() {}
Ray::Ray(Vector3 *origin, Vector3 *direction)
{
    this->origin.set(origin->x, origin->y, origin->z);
    this->direction.set(direction->x, direction->y, direction->z);
}

Ray::~Ray()
{
}

void Ray::set(Vector3 origin, Vector3 direction)
{
    this->origin.set(origin);
    this->direction.set(direction);
}

void Ray::setOrigin(Vector3 origin)
{
    this->origin.set(origin);
}

void Ray::setOrigin(const float &t_x, const float &t_y, const float &t_z)
{
    this->origin.set(t_x, t_y, t_z);
}

void Ray::setDirection(Vector3 direction)
{
    this->direction.set(direction);
}

void Ray::setDirection(const float &t_x, const float &t_y, const float &t_z)
{
    this->direction.set(t_x, t_y, t_z);
}

float Ray::distanceToPoint(Vector3 point)
{
    return origin.distanceTo(point);
}

Vector3 Ray::at(float t)
{
    return (this->direction * t) + this->origin;
}

Vector3 Ray::intersectBox(Vector3 *minCorner, Vector3 *maxCorner)
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    const float invdirx = 1 / this->direction.x,
                invdiry = 1 / this->direction.y,
                invdirz = 1 / this->direction.z;

    const Vector3 origin = this->origin;

    if (invdirx >= 0)
    {

        tmin = (minCorner->x - origin.x) * invdirx;
        tmax = (maxCorner->x - origin.x) * invdirx;
    }
    else
    {

        tmin = (maxCorner->x - origin.x) * invdirx;
        tmax = (minCorner->x - origin.x) * invdirx;
    }

    if (invdiry >= 0)
    {

        tymin = (minCorner->y - origin.y) * invdiry;
        tymax = (maxCorner->y - origin.y) * invdiry;
    }
    else
    {

        tymin = (maxCorner->y - origin.y) * invdiry;
        tymax = (minCorner->y - origin.y) * invdiry;
    }

    if ((tmin > tymax) || (tymin > tmax))
        return Vector3();

    // These lines also handle the case where tmin or tmax is NaN
    // (result of 0 * Infinity). x !== x returns true if x is NaN

    if (tymin > tmin || tmin != tmin)
        tmin = tymin;

    if (tymax < tmax || tmax != tmax)
        tmax = tymax;

    if (invdirz >= 0)
    {

        tzmin = (minCorner->z - origin.z) * invdirz;
        tzmax = (maxCorner->z - origin.z) * invdirz;
    }
    else
    {

        tzmin = (maxCorner->z - origin.z) * invdirz;
        tzmax = (minCorner->z - origin.z) * invdirz;
    }

    if ((tmin > tzmax) || (tzmin > tmax))
        return Vector3();

    if (tzmin > tmin || tmin != tmin)
        tmin = tzmin;

    if (tzmax < tmax || tmax != tmax)
        tmax = tzmax;

    // return point closest to the ray (positive side)

    if (tmax < 0)
        return Vector3();

    return this->at(tmin >= 0 ? tmin : tmax);
}

Vector3 Ray::invDir(){
    return Vector3(
        1 / this->direction.x,
        1 / this->direction.y,
        1 / this->direction.z
    );
}