/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/models/mesh_frame.hpp"
#include "../include/utils/debug.hpp"

// ----
// Constructors/Destructors
// ----

MeshFrame::MeshFrame()
{
    vertexCount = 0;
    stsCount = 0;
    normalsCount = 0;
    materialsCount = 0;
    _areSTsAllocated = false;
    _areVerticesAllocated = false;
    _areNormalsAllocated = false;
    _areMaterialsAllocated = false;
}

MeshFrame::~MeshFrame()
{
    if (_areSTsAllocated)
        delete[] sts;
    if (_areVerticesAllocated)
        delete[] vertices;
    if (_areNormalsAllocated)
        delete[] normals;
    if (_areMaterialsAllocated)
        delete[] materials;
}

// ----
// Methods
// ----

void MeshFrame::allocateSTs(const u32 &t_val)
{
    if (_areSTsAllocated)
    {
        PRINT_ERR("Can't allocate STs, because were already set!");
        return;
    }
    stsCount = t_val;
    sts = new Point[t_val];
    _areSTsAllocated = true;
}

void MeshFrame::allocateVertices(const u32 &t_val)
{
    if (_areVerticesAllocated)
    {
        PRINT_ERR("Can't allocate vertices, because were already set!");
        return;
    }
    vertexCount = t_val;
    vertices = new Vector3[t_val];
    _areVerticesAllocated = true;
}

void MeshFrame::allocateNormals(const u32 &t_val)
{
    if (_areNormalsAllocated)
    {
        PRINT_ERR("Can't allocate normals, because were already set!");
        return;
    }
    normalsCount = t_val;
    normals = new Vector3[t_val];
    _areNormalsAllocated = true;
}

void MeshFrame::allocateMaterials(const u32 &t_val)
{
    if (_areMaterialsAllocated)
    {
        PRINT_ERR("Can't allocate materials, because were already set!");
        return;
    }
    materialsCount = t_val;
    materials = new MeshMaterial[t_val];
    _areMaterialsAllocated = true;
}

void MeshFrame::calculateBoundingBox()
{
    if (!_areVerticesAllocated)
    {
        PRINT_ERR("Can't calculate bounding box, because vertices was not allocated!");
        return;
    }
    float lowX, lowY, lowZ, hiX, hiY, hiZ;
    lowX = hiX = vertices[0].x;
    lowY = hiY = vertices[0].y;
    lowZ = hiZ = vertices[0].z;
    for (u32 i = 0; i < vertexCount; i++)
    {
        if (lowX > vertices[i].x)
            lowX = vertices[i].x;
        if (hiX < vertices[i].x)
            hiX = vertices[i].x;

        if (lowY > vertices[i].y)
            lowY = vertices[i].y;
        if (hiY < vertices[i].y)
            hiY = vertices[i].y;

        if (lowZ > vertices[i].z)
            lowZ = vertices[i].z;
        if (hiZ < vertices[i].z)
            hiZ = vertices[i].z;
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
