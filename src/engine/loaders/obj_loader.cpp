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

void ObjLoader::load(MeshFrame *o_result, char *t_filename, float t_scale, u8 t_invertT)
{
    char *path = String::createConcatenated("host:", t_filename);
    FILE *file = fopen(path, "rb");
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
                o_result->setVertex(verticesI++, vector * t_scale);
            }
            else if (strcmp(lineHeader, "vt") == 0)
            {
                Point point = Point();
                fscanf(file, "%f %f\n", &point.x, &point.y);
                if (t_invertT)
                    point.y = 1.0F - point.y;
                o_result->setST(cordsI++, point);
            }
            else if (strcmp(lineHeader, "vn") == 0)
            {
                Vector3 vector = Vector3();
                fscanf(file, "%f %f %f\n", &vector.x, &vector.y, &vector.z);
                o_result->setNormal(normalsI++, vector);
            }
            else if (strcmp(lineHeader, "usemtl") == 0)
            {
                char temp[30];
                fscanf(file, "%s\n", temp);
                o_result->getMaterial(++materialsI).setName(temp);
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
                    o_result->getMaterial(materialsI).setVertexFace(faceI, vertexIndex[0] - 1);
                    o_result->getMaterial(materialsI).setVertexFace(faceI + 1, vertexIndex[1] - 1);
                    o_result->getMaterial(materialsI).setVertexFace(faceI + 2, vertexIndex[2] - 1);

                    o_result->getMaterial(materialsI).setSTFace(faceI, coordIndex[0] - 1);
                    o_result->getMaterial(materialsI).setSTFace(faceI + 1, coordIndex[1] - 1);
                    o_result->getMaterial(materialsI).setSTFace(faceI + 2, coordIndex[2] - 1);

                    o_result->getMaterial(materialsI).setNormalFace(faceI, normalIndex[0] - 1);
                    o_result->getMaterial(materialsI).setNormalFace(faceI + 1, normalIndex[1] - 1);
                    o_result->getMaterial(materialsI).setNormalFace(faceI + 2, normalIndex[2] - 1);
                    faceI += 3;
                }
            }
        }
        else
            break;
    }
    o_result->calculateBoundingBoxes();
    fclose(file);
    delete[] path;
}

/** Calculate how many vertices(v), coordinates(vt), normals(vn) and faces(f) have .obj file */
void ObjLoader::allocateObjMemory(FILE *t_file, MeshFrame *o_result)
{
    u32 vertexCount = 0;
    u32 stsCount = 0;
    u32 normalsCount = 0;
    u32 materialsCount = 0;

    while (1)
    {
        char lineHeader[128];
        int res = fscanf(t_file, "%s", lineHeader);
        if (res != EOF)
        {
            if (strcmp(lineHeader, "v") == 0)
                vertexCount += 1;
            else if (strcmp(lineHeader, "vt") == 0)
                stsCount += 1;
            else if (strcmp(lineHeader, "vn") == 0)
                normalsCount += 1;
            else if (strcmp(lineHeader, "usemtl") == 0)
                materialsCount += 1;
        }
        else
            break;
    }

    o_result->allocateVertices(vertexCount);
    o_result->allocateNormals(normalsCount);
    o_result->allocateSTs(stsCount);
    o_result->allocateMaterials(materialsCount);

    s16 currentMatI = -1;

    fseek(t_file, 0, SEEK_SET);
    u32 facesCounter = 0;
    while (1)
    {
        char lineHeader[128];
        int res = fscanf(t_file, "%s", lineHeader);
        if (res != EOF)
        {
            if (strcmp(lineHeader, "usemtl") == 0)
            {
                if (currentMatI >= 0) // Skip -1
                {
                    o_result->getMaterial(currentMatI).allocateFaces(facesCounter);
                    facesCounter = 0;
                }
                currentMatI++;
            }
            else if (strcmp(lineHeader, "f") == 0)
                facesCounter += 3;
        }
        else
            break;
    }

    o_result->getMaterial(currentMatI).allocateFaces(facesCounter); // Allocate last one

    // o_result->isMemoryAllocated = true;
}
