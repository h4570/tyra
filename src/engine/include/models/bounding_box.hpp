/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Michał Mostowik <mostek3pl@gmail.com>
*/

#ifndef _TYRA_BOUNDING_BOX_
#define _TYRA_BOUNDING_BOX_

#include <stddef.h>
#include <tamtypes.h>
#include "math/point.hpp"
#include "math/vector3.hpp"

/**
 * Struct containing face data used in
 * bounding box class.
 */
struct BoundingBoxFace
{
    BoundingBoxFace(Vector3 t_minCorner, Vector3 t_maxCorner, float t_axisPos)
    {
        minCorner = t_minCorner;
        maxCorner = t_maxCorner;
        axisPosition = t_axisPos;
    }
    /** Position of the face on it's respective axis. */
    float axisPosition;
    /** Corner of the face with lower coordinates */
    Vector3 minCorner;
    /** Corner of the face with higher coordinates */
    Vector3 maxCorner;
};

/**
 * Class containing bounding box data
 * which can be used for collision detection.
 */

class BoundingBox
{
public:
    BoundingBox(Vector3 *t_vertices);
    ~BoundingBox();
    const float &getHeight() { return _height; };
    const float &getDepth() { return _depth; };
    const float &getWidth() { return _width; };
    /** Return the vector directly in middle of the bounding box. */
    const Vector3 &getCenter() { return _centerVector; };
    /** Return the front face (furhter on z-axis) */
    const BoundingBoxFace &getFrontFace() { return _frontFace; };

private:
    Vector3 _vertices[8];
    float _height, _depth, _width;
    Vector3 _centerVector;
    BoundingBoxFace _frontFace;
    BoundingBoxFace _backFace;
    BoundingBoxFace _leftFace;
    BoundingBoxFace _rightFace;
    BoundingBoxFace _topFace;
    BoundingBoxFace _bottomFace;
};

#endif