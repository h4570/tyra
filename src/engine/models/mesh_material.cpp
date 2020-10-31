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
