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
    /** @returns the vector directly in middle of the bounding box. */
    const Vector3 &getCenter() { return _centerVector; };
    /** @returns the front face (further on z-axis) */
    const BoundingBoxFace &getFrontFace() const { return _frontFace; };
    /** @returns the back face (nearer on z-axis) */
    const BoundingBoxFace &getBackFace() const { return _backFace; };
    /** @returns the left face (further on x-axis) */
    const BoundingBoxFace &getLeftFace() const { return _leftFace; };
    /** @returns the right face (nearer on x-axis) */
    const BoundingBoxFace &getRightFace() const { return _rightFace; };
    /** @returns the top face (further on y-axis) */
    const BoundingBoxFace &getTopFace() const { return _topFace; };
    /** @returns the bottom face (nearer on y-axis) */
    const BoundingBoxFace &getBottomFace() const { return _bottomFace; };

    /** @returns check's intersection against another AABB */
    bool intersect(const BoundingBox &b, const Vector3 &aPosition, const Vector3 &bPosition) const
    {
        const Vector3 bMax = b.getVertex(7);
        const Vector3 bMin = b.getVertex(0);
        return ((_vertices[0].x + aPosition.x) <= (bMax.x + bPosition.x) && (_vertices[7].x + aPosition.x) >= (bMin.x + bPosition.x)) &&
               ((_vertices[0].y + aPosition.y) <= (bMax.y + bPosition.y) && (_vertices[7].y + aPosition.y) >= (bMin.y + bPosition.y)) &&
               ((_vertices[0].z + aPosition.z) <= (bMax.z + bPosition.z) && (_vertices[7].z + aPosition.z) >= (bMin.z + bPosition.z));
    }

    /** 
     * @returns bounding box raw vertices array.
     * Total length: 8
     */
    const Vector3 &getVertex(const u8 &i) const { return _vertices[i]; };
    /** @returns single vertex from raw vertices array.*/
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
