/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Welligton Carvalho <wellcoj@gmail.com>
*/

#ifndef _TYRA_RAY_
#define _TYRA_RAY_

#include "vector3.hpp"
#include "plane.hpp"
#include "../bounding_box.hpp"
#include <tamtypes.h>
#include <math.h>
#include <algorithm>

/**
 * Class for raycasting.
 */
class Ray
{

public:
    /**
     * Constructor create a ampty ray
     *
     */
    Ray();

    /**
     * Constructor
     * @param origin The origin of the ray
     * @param direction The direction of the ray (normalized Vector3)
     *
     */
    Ray(Vector3 *origin, Vector3 *direction);
    ~Ray();

    Vector3 origin;
    Vector3 direction;

    // Methods

    /**
     * Set origin and direction
     */
    void set(Vector3 origin, Vector3 direction);

    /**
     * Set origin only
     */
    void setOrigin(Vector3 origin);
    void setOrigin(const float &t_x, const float &t_y, const float &t_z);

    /**
     * Set direction only
     */
    void setDirection(Vector3 origin);
    void setDirection(const float &t_x, const float &t_y, const float &t_z);

    /**
     * Return Vector3 that is a given distance along this Ray
     * @param t - the distance along the Ray to retrieve a position for.
     */
    Vector3 at(float t);

    /**
     * Return the distance from the Vector3 point to the origin
     */
    float distanceToPoint(Vector3 point);

    /**
     * Returns distance from origin to intersected mesh
     */
    float distanceTo(const Vector3 &v) const;

    /**
     * Returns Vector3 of intersection position
     * @param minCorner - pointer to min corner position. (bottom left)
     * @param maxCorner - pointer to max corner position. (top right)
     * @param distance - the box to intersect with. 
     *
     */
    u8 intersectBox(Vector3 *minCorner, Vector3 *maxCorner, float &distance);

    /**
     * Returns distance from origin to intersected mesh
     */
    inline const Vector3 getPosition() { return _position; };

    /**
     * Returns inverse direction
     */
    Vector3 invDir();

private:
    Vector3 _position;
};

#endif
