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
    BoundingBoxFace(){

    };
    BoundingBoxFace(const Vector3 &t_minCorner, const Vector3 &t_maxCorner, float t_axisPos)
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
    /** Return the front face (further on z-axis) */
    const BoundingBoxFace &getFrontFace() { return _frontFace; };
    /** Return the back face (nearer on z-axis) */
    const BoundingBoxFace &getBackFace() { return _backFace; };
    /** Return the left face (further on x-axis) */
    const BoundingBoxFace &getLeftFace() { return _leftFace; };
    /** Return the right face (nearer on x-axis) */
    const BoundingBoxFace &getRightFace() { return _rightFace; };
    /** Return the top face (further on y-axis) */
    const BoundingBoxFace &getTopFace() { return _topFace; };
    /** Return the bottom face (nearer on y-axis) */
    const BoundingBoxFace &getBottomFace() { return _bottomFace; };
    /** 
     * Returns bounding box raw vertices array.
     * Total length: 8
     */
    Vector3 &getVertex(const u8 &i) { return _vertices[i]; };
    /** Returns single vertex from raw vertices array.*/
    Vector3 *getVertices() { return _vertices; };

private:
    Vector3 _vertices[8];
    float _height, _depth, _width;
    Vector3 _centerVector;
    /** Front face (further on z-axis) */
    BoundingBoxFace _frontFace;
    /** Back face (nearer on z-axis) */
    BoundingBoxFace _backFace;
    /** Left face (nearer on x-axis) */
    BoundingBoxFace _leftFace;
    /** Right face (further on x-axis) */
    BoundingBoxFace _rightFace;
    /** Top face (further on y-axis) */
    BoundingBoxFace _topFace;
    /** Bottom face (nearer on y-axis) */
    BoundingBoxFace _bottomFace;
};

#endif