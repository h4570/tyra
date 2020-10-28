/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/models/obj_model.hpp"

#include "../include/utils/debug.hpp"
#include "../include/utils/string.hpp"

// ----
// Constructors/Destructors
// ----

ObjModel::ObjModel(char *t_objFile)
{
    filename = String::createWithoutExtension(t_objFile);
}

ObjModel::~ObjModel()
{
    if (isMemoryAllocated == true)
    {
        delete[] vertices;
        delete[] coordinates;
        delete[] normals;
        for (u16 i = 0; i < materialsCount; i++)
        {
            delete[] materials[i].verticeFaces;
            delete[] materials[i].coordinateFaces;
            delete[] materials[i].normalFaces;
            delete[] materials[i].materialName;
        }
        delete[] materials;
    }
}

// ----
// Methods
// ----

u32 ObjModel::getFacesCount()
{
    u32 result = 0;
    for (u16 i = 0; i < materialsCount; i++)
        result += materials[i].facesCount;
    return result;
}

u32 ObjModel::getDrawData(u32 t_materialIndex, VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, Vector3 &t_cameraPos, float t_scale, u8 t_shouldBeBackfaceCulled)
{
    u32 addedFaces = 0;
    for (u32 j = 0; j < materials[t_materialIndex].facesCount; j += 3)
        if (!t_shouldBeBackfaceCulled || Vector3::shouldBeBackfaceCulled(
                                             &t_cameraPos,
                                             &vertices[materials[t_materialIndex].verticeFaces[j]],
                                             &vertices[materials[t_materialIndex].verticeFaces[j + 1]],
                                             &vertices[materials[t_materialIndex].verticeFaces[j + 2]]) == 0)
        {
            fillNextFace(o_vertices, o_normals, o_coordinates, o_colors, t_materialIndex, j, addedFaces++);
            fillNextFace(o_vertices, o_normals, o_coordinates, o_colors, t_materialIndex, j + 1, addedFaces++);
            fillNextFace(o_vertices, o_normals, o_coordinates, o_colors, t_materialIndex, j + 2, addedFaces++);
        }
    return addedFaces;
}

void ObjModel::fillNextFace(VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, u32 t_materialI, u32 t_getI, u32 t_setI)
{
    o_vertices[t_setI][0] = vertices[materials[t_materialI].verticeFaces[t_getI]].x;
    o_vertices[t_setI][1] = vertices[materials[t_materialI].verticeFaces[t_getI]].y;
    o_vertices[t_setI][2] = vertices[materials[t_materialI].verticeFaces[t_getI]].z;
    o_vertices[t_setI][3] = 1.0F;

    o_normals[t_setI][0] = normals[materials[t_materialI].normalFaces[t_getI]].x;
    o_normals[t_setI][1] = normals[materials[t_materialI].normalFaces[t_getI]].y;
    o_normals[t_setI][2] = normals[materials[t_materialI].normalFaces[t_getI]].z;
    o_normals[t_setI][3] = 1.0F;

    o_coordinates[t_setI][0] = coordinates[materials[t_materialI].coordinateFaces[t_getI]].x;
    o_coordinates[t_setI][1] = coordinates[materials[t_materialI].coordinateFaces[t_getI]].y;
    o_coordinates[t_setI][2] = 1.0F;
    o_coordinates[t_setI][3] = 1.0F;

    o_colors[t_setI][0] = 1.0F;
    o_colors[t_setI][1] = 1.0F;
    o_colors[t_setI][2] = 1.0F;
    o_colors[t_setI][3] = 1.0F;
}
