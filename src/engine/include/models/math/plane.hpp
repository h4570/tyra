/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_PLANE_
#define _TYRA_PLANE_

#include "vector3.hpp"

/** https://en.wikipedia.org/wiki/Plane_(geometry) */
class Plane
{

public:
    Vector3 normal;
    float distance;

    Plane();
    Plane(Vector3 &a, Vector3 &b, Vector3 &c);
    ~Plane();

    void update(Vector3 &a, Vector3 &b, Vector3 &c);
    inline float distanceTo(Vector3 &t_vec) { return this->distance + this->normal.innerProduct(t_vec); }
    void print();
};

#endif
