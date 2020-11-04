/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../../include/models/math/plane.hpp"

#include <stdio.h>

// ----
// Constructors/Destructors
// ----

/** Create empty plane */
Plane::Plane() { this->distance = 0; }

/** Create by specyfying 3 points.
 * This function assumes that the points
 * are given in counter clockwise order
 */
Plane::Plane(Vector3 &a, Vector3 &b, Vector3 &c) { this->update(a, b, c); }

Plane::~Plane() {}

// ----
// Methods
// ----

/** Set plane by specyfying 3 points.
 * This function assumes that the points
 * are given in counter clockwise order
 */
void Plane::update(Vector3 &a, Vector3 &b, Vector3 &c)
{
    Vector3 aux1 = a - b;
    Vector3 aux2 = c - b;
    this->normal = aux2 * aux1;
    this->normal.normalize();
    this->distance = -this->normal.innerProduct(b);
}

const void Plane::print() const
{
    printf("Plane(Vector3(%f, %f, %f), %f)\n", this->normal.x, this->normal.y, this->normal.z, this->distance);
}
