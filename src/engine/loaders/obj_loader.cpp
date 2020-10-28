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
#include <string.h>

// ----
// Constructors/Destructors
// ----

ObjLoader::ObjLoader() {}

ObjLoader::~ObjLoader() {}

// ----
// Methods
// ----

/** Load, parse .obj file and load data into given MeshSpec */
void ObjLoader::load(ObjModel *o_result, char *t_filename, float t_scale)
{
    FILE *file = fopen(t_filename, "rb");
    if (file == NULL)
        PRINT_ERR("Failed to load .obj file!");
    setObjDataQuantities(file, o_result);
    o_result->allocateMemory();
    u32 verticesI = 0, cordsI = 0, normalsI = 0, i = 0, vertexIndex[3], coordIndex[3], normalIndex[3];
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
                    o_result->verticeFaces[i] = vertexIndex[0] - 1;
                    o_result->verticeFaces[i + 1] = vertexIndex[1] - 1;
                    o_result->verticeFaces[i + 2] = vertexIndex[2] - 1;

                    o_result->coordinateFaces[i] = coordIndex[0] - 1;
                    o_result->coordinateFaces[i + 1] = coordIndex[1] - 1;
                    o_result->coordinateFaces[i + 2] = coordIndex[2] - 1;

                    o_result->normalFaces[i] = normalIndex[0] - 1;
                    o_result->normalFaces[i + 1] = normalIndex[1] - 1;
                    o_result->normalFaces[i + 2] = normalIndex[2] - 1;
                    i += 3;
                }
            }
        }
        else
            break;
    }
    fclose(file);
}

/** Calculate how many vertices(v), coordinates(vt), normals(vn) and faces(f) have .obj file
 * When done, file read offset is resetted to the begginning of file
 */
void ObjLoader::setObjDataQuantities(FILE *t_file, ObjModel *o_result)
{
    o_result->verticesCount = 0;
    o_result->coordinatesCount = 0;
    o_result->normalsCount = 0;
    o_result->facesCount = 0;
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
            else if (strcmp(lineHeader, "f") == 0)
                o_result->facesCount += 3;
        }
        else
            break;
    }
    fseek(t_file, 0, SEEK_SET);
}
