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

#include <stdio.h>

// ----
// Constructors/Destructors
// ----

/** Create by specifying values */
Point::Point(float x, float y)
{
    this->x = x;
    this->y = y;
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

void Point::set(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Point::print()
{
    printf("Point(%f, %f)\n", x, y);
}
