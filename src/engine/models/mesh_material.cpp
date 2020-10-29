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

// ----
// Constructors/Destructors
// ----

MeshMaterial::MeshMaterial()
{
    facesCount = 0;
    isNameAllocated = false;
    areFacesAllocated = false;
}

MeshMaterial::~MeshMaterial()
{

    if (areFacesAllocated)
    {
        delete[] vertexFaces;
        delete[] stFaces;
        delete[] normalFaces;
    }
    if (isNameAllocated)
    {
        delete[] name;
    }
}

// ----
// Methods
// ----

void MeshMaterial::setFacesCount(const u32 &t_val)
{
    if (areFacesAllocated)
    {
        PRINT_ERR("Can't set faces, because faces were already set!");
        return;
    }
    facesCount = t_val;
    stFaces = new u32[t_val];
    normalFaces = new u32[t_val];
    vertexFaces = new u32[t_val];
    areFacesAllocated = true;
}

void MeshMaterial::setName(char *t_val)
{
    if (isNameAllocated)
    {
        PRINT_ERR("Can't set name, because was already set!");
        return;
    }
    name = String::createCopy(t_val);
    isNameAllocated = true;
}
