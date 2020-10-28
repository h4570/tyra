/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/loaders/obj_loader.hpp"

#include "../include/utils/debug.hpp"
#include "../include/utils/string.hpp"
#include <string.h>

// ----
// Constructors/Destructors
// ----

ObjLoader::ObjLoader() {}

ObjLoader::~ObjLoader() {}

// ----
// Methods
// ----

/** Parse .obj file, allocate output data and store it. Multitexture support
 * Notice: At this moment textures names are MATERIAL names from .obj file!
 * Notice 2: Faces MUST be triangulated (check out blender export settings).
 */
void ObjLoader::load(Frame *o_result, char *t_filename, float t_scale)
{
    FILE *file = fopen(t_filename, "rb");
    if (file == NULL)
        PRINT_ERR("Failed to load .obj file!");
    allocateObjMemory(file, o_result);
    fseek(file, 0, SEEK_SET);
    u32 verticesI = 0, cordsI = 0, normalsI = 0, faceI = 0, vertexIndex[3], coordIndex[3], normalIndex[3];
    s16 materialsI = -1;
    while (1)
    {
        char lineHeader[128]; // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res != EOF)
        {
            if (strcmp(lineHeader, "v") == 0)
            {
                Vector3 vector = Vector3();
                fscanf(file, "%f %f %f\n", &vector.x, &vector.y, &vector.z);
                o_result->vertices[verticesI++] = vector * t_scale;
            }
            else if (strcmp(lineHeader, "vt") == 0)
            {
                Vector3 vector = Vector3();
                fscanf(file, "%f %f\n", &vector.x, &vector.y);
                o_result->coordinates[cordsI++] = vector;
            }
            else if (strcmp(lineHeader, "vn") == 0)
            {
                Vector3 vector = Vector3();
                fscanf(file, "%f %f %f\n", &vector.x, &vector.y, &vector.z);
                o_result->normals[normalsI++] = vector;
            }
            else if (strcmp(lineHeader, "usemtl") == 0)
            {
                char temp[30];
                fscanf(file, "%s\n", temp);
                o_result->materials[++materialsI].materialName = String::createCopy(temp);
                faceI = 0;
            }
            else if (strcmp(lineHeader, "f") == 0)
            {
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                                     &vertexIndex[0], &coordIndex[0], &normalIndex[0],
                                     &vertexIndex[1], &coordIndex[1], &normalIndex[1],
                                     &vertexIndex[2], &coordIndex[2], &normalIndex[2]);
                if (matches != 9)
                    PRINT_ERR(".obj can't be read by this simple parser. Try exporting with other options");
                else
                {
                    o_result->materials[materialsI].verticeFaces[faceI] = vertexIndex[0] - 1;
                    o_result->materials[materialsI].verticeFaces[faceI + 1] = vertexIndex[1] - 1;
                    o_result->materials[materialsI].verticeFaces[faceI + 2] = vertexIndex[2] - 1;

                    o_result->materials[materialsI].coordinateFaces[faceI] = coordIndex[0] - 1;
                    o_result->materials[materialsI].coordinateFaces[faceI + 1] = coordIndex[1] - 1;
                    o_result->materials[materialsI].coordinateFaces[faceI + 2] = coordIndex[2] - 1;

                    o_result->materials[materialsI].normalFaces[faceI] = normalIndex[0] - 1;
                    o_result->materials[materialsI].normalFaces[faceI + 1] = normalIndex[1] - 1;
                    o_result->materials[materialsI].normalFaces[faceI + 2] = normalIndex[2] - 1;
                    faceI += 3;
                }
            }
        }
        else
            break;
    }
    fclose(file);
}

/** Calculate how many vertices(v), coordinates(vt), normals(vn) and faces(f) have .obj file */
void ObjLoader::allocateObjMemory(FILE *t_file, Frame *o_result)
{
    o_result->verticesCount = 0;
    o_result->coordinatesCount = 0;
    o_result->normalsCount = 0;
    o_result->materialsCount = 0;

    while (1)
    {
        char lineHeader[128];
        int res = fscanf(t_file, "%s", lineHeader);
        if (res != EOF)
        {
            if (strcmp(lineHeader, "v") == 0)
                o_result->verticesCount += 1;
            else if (strcmp(lineHeader, "vt") == 0)
                o_result->coordinatesCount += 1;
            else if (strcmp(lineHeader, "vn") == 0)
                o_result->normalsCount += 1;
            else if (strcmp(lineHeader, "usemtl") == 0)
                o_result->materialsCount += 1;
        }
        else
            break;
    }

    o_result->materials = new ObjMaterial[o_result->materialsCount];
    s16 currentMatI = -1;

    fseek(t_file, 0, SEEK_SET);

    while (1)
    {
        char lineHeader[128];
        int res = fscanf(t_file, "%s", lineHeader);
        if (res != EOF)
        {
            if (strcmp(lineHeader, "usemtl") == 0)
                o_result->materials[++currentMatI].facesCount = 0;
            else if (strcmp(lineHeader, "f") == 0)
                o_result->materials[currentMatI].facesCount += 3;
        }
        else
            break;
    }

    for (u16 i = 0; i < o_result->materialsCount; i++)
    {
        o_result->materials[i].coordinateFaces = new u32[o_result->materials[i].facesCount];
        o_result->materials[i].normalFaces = new u32[o_result->materials[i].facesCount];
        o_result->materials[i].verticeFaces = new u32[o_result->materials[i].facesCount];
    }
    o_result->vertices = new Vector3[o_result->verticesCount];
    o_result->coordinates = new Vector3[o_result->coordinatesCount];
    o_result->normals = new Vector3[o_result->normalsCount];
    // o_result->isMemoryAllocated = true;
}
