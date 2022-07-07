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

u8 Ray::intersectBox(Vector3 *minCorner, Vector3 *maxCorner, float &distance)
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    Vector3 invDir = this->invDir();
    invDir.normalize();

    tmin = (minCorner->x - this->origin.x) * invDir.x;
    tmax = (maxCorner->x - this->origin.x) * invDir.x;
    tymin = (minCorner->y - this->origin.y) * invDir.y;
    tymax = (maxCorner->y - this->origin.y) * invDir.y;

    if ((tmin > tymax) || (tymin > tmax))
    {
        distance = -1.0f;
        return 0;
    }

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    tzmin = (minCorner->z - this->origin.z) * invDir.z;
    tzmax = (maxCorner->z - this->origin.z) * invDir.z;

    if ((tmin > tzmax) || (tzmin > tmax))
    {
        distance = -1.0f;
        return 0;
    }

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    if (tmax < 0)
    {
        distance = -1.0f;
        return 0;
    }

    distance = tmin >= 0 ? tmin : tmax;
    return 1;
}

Vector3 Ray::invDir()
{
    return Vector3(
        1 / this->direction.x,
        1 / this->direction.y,
        1 / this->direction.z);
}
