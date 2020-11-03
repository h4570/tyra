/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/models/mesh_material.hpp"
#include "../include/utils/debug.hpp"
#include "../include/utils/string.hpp"
#include <cstdlib>

// ----
// Constructors/Destructors
// ----

MeshMaterial::MeshMaterial()
{
    id = rand() % 1000000;
    facesCount = 0;
    _isNameSet = false;
    _areFacesAllocated = false;
    _isBoundingBoxCalculated = false;
}

MeshMaterial::~MeshMaterial()
{
    if (_areFacesAllocated)
    {
        delete[] vertexFaces;
        delete[] stFaces;
        delete[] normalFaces;
    }
    if (_isNameSet)
        delete[] name;
}

// ----
// Methods
// ----

void MeshMaterial::allocateFaces(const u32 &t_val)
{
    if (_areFacesAllocated)
    {
        PRINT_ERR("Can't allocate faces, because were already set!");
        return;
    }
    facesCount = t_val;
    stFaces = new u32[t_val];
    normalFaces = new u32[t_val];
    vertexFaces = new u32[t_val];
    _areFacesAllocated = true;
}

void MeshMaterial::setName(char *t_val)
{
    if (_isNameSet)
    {
        PRINT_ERR("Can't set name, because was already set!");
        return;
    }
    name = String::createCopy(t_val);
    _isNameSet = true;
}

u8 MeshMaterial::isInFrustum(Plane *t_frustumPlanes, const Vector3 &position)
{
    Vector3 boxCalcTemp;
    u8 boxResult = 1, boxIn = 0, boxOut = 0;
    for (int i = 0; i < 6; i++)
    {
        boxOut = 0;
        boxIn = 0;
        // for each corner of the box do ...
        // get out of the cycle as soon as a box as corners
        // both inside and out of the frustum
        for (int y = 0; y < 8 && (boxIn == 0 || boxOut == 0); y++)
        {
            boxCalcTemp.set(
                boundingBox[y].x + position.x,
                boundingBox[y].y + position.y,
                boundingBox[y].z + position.z);
            if (t_frustumPlanes[i].distanceTo(boxCalcTemp) < 0)
                boxOut++;
            else
                boxIn++;
        }
        //if all corners are out
        if (boxIn == 0)
            return 0;
        else if (boxOut)
            boxResult = 1;
    }
    return boxResult;
}

void MeshMaterial::calculateBoundingBox(Vector3 *t_vertices, u32 t_vertCount)
{
    float lowX, lowY, lowZ, hiX, hiY, hiZ;
    lowX = hiX = t_vertices[vertexFaces[0]].x;
    lowY = hiY = t_vertices[vertexFaces[0]].y;
    lowZ = hiZ = t_vertices[vertexFaces[0]].z;
    for (u32 i = 0; i < facesCount; i++)
    {
        if (lowX > t_vertices[vertexFaces[i]].x)
            lowX = t_vertices[vertexFaces[i]].x;
        if (hiX < t_vertices[vertexFaces[i]].x)
            hiX = t_vertices[vertexFaces[i]].x;

        if (lowY > t_vertices[vertexFaces[i]].y)
            lowY = t_vertices[vertexFaces[i]].y;
        if (hiY < t_vertices[vertexFaces[i]].y)
            hiY = t_vertices[vertexFaces[i]].y;

        if (lowZ > t_vertices[vertexFaces[i]].z)
            lowZ = t_vertices[vertexFaces[i]].z;
        if (hiZ < t_vertices[vertexFaces[i]].z)
            hiZ = t_vertices[vertexFaces[i]].z;
    }
    boundingBox[0].set(lowX, lowY, lowZ);
    boundingBox[1].set(lowX, lowY, hiZ);
    boundingBox[2].set(lowX, hiY, lowZ);
    boundingBox[3].set(lowX, hiY, hiZ);

    boundingBox[4].set(hiX, lowY, lowZ);
    boundingBox[5].set(hiX, lowY, hiZ);
    boundingBox[6].set(hiX, hiY, lowZ);
    boundingBox[7].set(hiX, hiY, hiZ);
    _isBoundingBoxCalculated = true;
}
