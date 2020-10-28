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

        delete[] verticeFaces;
        delete[] coordinateFaces;
        delete[] normalFaces;
    }
}

// ----
// Methods
// ----

u32 ObjModel::getDrawData(VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, Vector3 &t_cameraPos, float t_scale, u8 t_shouldBeBackfaceCulled)
{
    u32 addedFaces = 0;
    for (u32 i = 0; i < facesCount; i += 3)
        if (!t_shouldBeBackfaceCulled || Vector3::shouldBeBackfaceCulled(
                                             &t_cameraPos,
                                             &vertices[verticeFaces[i]],
                                             &vertices[verticeFaces[i + 1]],
                                             &vertices[verticeFaces[i + 2]]) == 0)
        {
            fillNextFace(o_vertices, o_normals, o_coordinates, o_colors, i, addedFaces++);
            fillNextFace(o_vertices, o_normals, o_coordinates, o_colors, i + 1, addedFaces++);
            fillNextFace(o_vertices, o_normals, o_coordinates, o_colors, i + 2, addedFaces++);
        }

    return addedFaces;
}

/** Allocate memory for vertices,coords etc. 
 * Called by ObjLoader
 */
void ObjModel::allocateMemory()
{
    PRINT_LOG("Allocating 3D object specification memory");
    vertices = new Vector3[verticesCount];
    coordinates = new Vector3[coordinatesCount];
    normals = new Vector3[normalsCount];

    verticeFaces = new u32[facesCount];
    coordinateFaces = new u32[facesCount];
    normalFaces = new u32[facesCount];
    isMemoryAllocated = true;
    PRINT_LOG("3D object specification memory allocated!");
}

void ObjModel::fillNextFace(VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, VECTOR *o_colors, u32 t_getI, u32 t_setI)
{
    o_vertices[t_setI][0] = vertices[verticeFaces[t_getI]].x;
    o_vertices[t_setI][1] = vertices[verticeFaces[t_getI]].y;
    o_vertices[t_setI][2] = vertices[verticeFaces[t_getI]].z;
    o_vertices[t_setI][3] = 1.0F;

    o_normals[t_setI][0] = normals[normalFaces[t_getI]].x;
    o_normals[t_setI][1] = normals[normalFaces[t_getI]].y;
    o_normals[t_setI][2] = normals[normalFaces[t_getI]].z;
    o_normals[t_setI][3] = 1.0F;

    o_coordinates[t_setI][0] = coordinates[coordinateFaces[t_getI]].x;
    o_coordinates[t_setI][1] = coordinates[coordinateFaces[t_getI]].y;
    o_coordinates[t_setI][2] = 1.0F;
    o_coordinates[t_setI][3] = 1.0F;

    o_colors[t_setI][0] = 1.0F;
    o_colors[t_setI][1] = 1.0F;
    o_colors[t_setI][2] = 1.0F;
    o_colors[t_setI][3] = 1.0F;
}
