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

BoundingBox::BoundingBox(Vector3 *t_vertices)
{
    //Perform a deep copy of vertex array parameter
    for (int i = 0; i < 8; i++)
        _vertices[i] = t_vertices[i];

    //This might be shortened with Vector3 operator overloading, but current
    //implementation is more human readable.
    _height = _vertices[0].y - _vertices[2].y;
    _width = _vertices[0].x - _vertices[4].x;
    _depth = _vertices[0].z - _vertices[1].z;

    _centerVector = _vertices[0];
    _centerVector.x += (_width / 2);
    _centerVector.y += (_height / 2);
    _centerVector.z += (_depth / 2);

    //Z-Axis faces
    _frontFace = BoundingBoxFace(_vertices[1], _vertices[7], _vertices[1].z);
    _backFace = BoundingBoxFace(_vertices[0], _vertices[6], _vertices[0].z);
    //X-Axis faces
    _leftFace = BoundingBoxFace(_vertices[0], _vertices[3], _vertices[0].x);
    _rightFace = BoundingBoxFace(_vertices[4], _vertices[7], _vertices[4].x);
    //Y-Axis faces
    _topFace = BoundingBoxFace(_vertices[2], _vertices[7], _vertices[2].y);
    _bottomFace = BoundingBoxFace(_vertices[0], _vertices[5], _vertices[0].y);
}

BoundingBox::~BoundingBox()
{
}