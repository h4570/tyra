/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <cstdlib>
#include "../include/models/mesh_frame.hpp"
#include "../include/utils/debug.hpp"

// ----
// Constructors/Destructors
// ----

MeshFrame::MeshFrame()
{
    id = rand() % 1000000;
    vertexCount = 0;
    stsCount = 0;
    normalsCount = 0;
    materialsCount = 0;
    _areSTsAllocated = false;
    _areVerticesAllocated = false;
    _areNormalsAllocated = false;
    _areMaterialsAllocated = false;
    _isMother = true;
}

MeshFrame::~MeshFrame()
{
    if (_isMother)
    {
        if (_areSTsAllocated)
            delete[] sts;
        if (_areVerticesAllocated)
            delete[] vertices;
        if (_areNormalsAllocated)
            delete[] normals;
        delete boundingBoxObj;
    }
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

void MeshFrame::calculateBoundingBoxes()
{
    if (!_areVerticesAllocated)
        PRINT_ERR("Can't calculate bounding box, because vertices were not allocated!");

    for (u32 i = 0; i < materialsCount; i++)
        materials->calculateBoundingBox(vertices, vertexCount);

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

    Vector3 boundingBox[8];
    boundingBox[0].set(lowX, lowY, lowZ);
    boundingBox[1].set(lowX, lowY, hiZ);
    boundingBox[2].set(lowX, hiY, lowZ);
    boundingBox[3].set(lowX, hiY, hiZ);

    boundingBox[4].set(hiX, lowY, lowZ);
    boundingBox[5].set(hiX, lowY, hiZ);
    boundingBox[6].set(hiX, hiY, lowZ);
    boundingBox[7].set(hiX, hiY, hiZ);
    _isBoundingBoxCalculated = true;

    // BoundingBox is declared on the heap to prevent any ill-formed default
    // constructor instantiated BoundingBox objects.
    boundingBoxObj = new BoundingBox(boundingBox);
}

void MeshFrame::copyFrom(MeshFrame *t_refCopy)
{
    vertexCount = t_refCopy->vertexCount;
    stsCount = t_refCopy->stsCount;
    normalsCount = t_refCopy->normalsCount;
    materialsCount = t_refCopy->materialsCount;
    materials = new MeshMaterial[materialsCount];
    for (u32 i = 0; i < materialsCount; i++)
        materials[i].copyFrom(&t_refCopy->materials[i]);
    _areSTsAllocated = true;
    _areVerticesAllocated = true;
    _areNormalsAllocated = true;
    _areMaterialsAllocated = true;
    vertices = t_refCopy->vertices;
    sts = t_refCopy->sts;
    normals = t_refCopy->normals;

    boundingBoxObj = t_refCopy->boundingBoxObj;
    _isBoundingBoxCalculated = true;

    _isMother = false;
}
