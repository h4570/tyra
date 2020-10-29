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
