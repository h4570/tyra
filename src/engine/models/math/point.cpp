/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../../include/models/math/point.hpp"
#include "../../include/utils/math.hpp"

#include <stdio.h>

// ----
// Constructors/Destructors
// ----

/** Create by specifying values */
Point::Point(const float &t_x, const float &t_y)
{
    x = t_x;
    y = t_y;
}

/** Create with other point values */
Point::Point(const Point &v)
{
    x = v.x;
    y = v.y;
}

/** Create empty point */
Point::Point()
{
    x = 0;
    y = 0;
}

Point::~Point() {}

// ----
// Methods
// ----

void Point::set(const float &t_x, const float &t_y)
{
    x = t_x;
    y = t_y;
}

void Point::set(const Point &v)
{
    x = v.x;
    y = v.y;
}

void Point::rotate(const float &t_angle, const float &t_x, const float &t_y)
{
    float s = Math::sin(t_angle);
    float c = Math::cos(t_angle);

    x -= t_x;
    y -= t_y;

    float xnew = x * c - y * s;
    float ynew = x * s + y * c;

    x = xnew + t_x;
    y = ynew + t_y;
}

const void Point::print() const
{
    printf("Point(%f, %f)\n", x, y);
}
