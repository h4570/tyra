/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#include "../include/models/bounding_box.hpp"
#include "../include/utils/debug.hpp"

BoundingBox::BoundingBox(Vector3 *t_vectorArray)
{
    if (sizeof(t_vectorArray) / sizeof(Vector3) != 8)
        PRINT_ERR("Attempted to create bounding box with invalid count of vectors!");

    //Perform a deep copy of vertex array parameter
    for (int i = 0; i < 9; i++)
        _vertices[i] = t_vectorArray[i];

    //This might be shortened with Vector3 operator overloading, but current
    //implementation is more human readable.
    _height = _vertices[0].y - _vertices[2].y;
    _width = _vertices[0].x - _vertices[4].x;
    _depth = _vertices[0].z - _vertices[1].z;

    _centerVector = _vertices[0];
    _centerVector.x += (_width / 2);
    _centerVector.y += (_height / 2);
    _centerVector.z += (_depth / 2);
}