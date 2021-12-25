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

void ObjLoader::load(MeshFrame *o_result, const char *t_filename, const float &t_scale, const u8 &t_invertT)
{
    char *path = String::createConcatenated("host:", t_filename);
    FILE *file = fopen(path, "rb");
    assertMsg(file != NULL, "Failed to load .obj file!");
    allocateObjMemory(file, o_result);
    fseek(file, 0, SEEK_SET);
    u32 verticesI = 0, cordsI = 0, normalsI = 0, faceI = 0, vertexIndex[3], coordIndex[3], normalIndex[3];
    s16 materialsI = -1;
    Vector3 vector = Vector3();
    Point point = Point();
    while (1)
    {
        char lineHeader[128]; // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res != EOF)
        {
            if (strcmp(lineHeader, "v") == 0)
            {
                fscanf(file, "%f %f %f\n", &vector.x, &vector.y, &vector.z);
                o_result->setVertex(verticesI++, vector * t_scale);
            }
            else if (strcmp(lineHeader, "vt") == 0)
            {
                fscanf(file, "%f %f\n", &point.x, &point.y);
                if (t_invertT)
                    point.y = 1.0F - point.y;
                o_result->setST(cordsI++, point);
            }
            else if (strcmp(lineHeader, "vn") == 0)
            {
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
                int *x = &res; // random assignment, to disable compilation warning
                fpos_t start;
                fgetpos(file, &start);
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", x, x, x, x, x, x, x, x, x);
                fsetpos(file, &start);
                int newerMatches = 0;
                switch (matches)
                {
                /** Vs, VTs and VNs all set */
                case 9:
                {
                    fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                           &vertexIndex[0], &coordIndex[0], &normalIndex[0],
                           &vertexIndex[1], &coordIndex[1], &normalIndex[1],
                           &vertexIndex[2], &coordIndex[2], &normalIndex[2]);
                }
                break;
                /** Loaded only two digits (V, VT) succesfuly. Not setting VN. */
                case 3:
                {
                    fscanf(file, "%d/%d/ %d/%d/ %d/%d/\n",
                           &vertexIndex[0], &coordIndex[0],
                           &vertexIndex[1], &coordIndex[1],
                           &vertexIndex[2], &coordIndex[2]);
                }
                break;
                /** Only V set. Checking for existance of VT or VN. */
                case 1:
                {
                    /** Check for existance of V/// configuration.. */
                    newerMatches = fscanf(file, "%d// %d// %d//\n", x, x, x);
                    fsetpos(file, &start);
                    if (newerMatches == 3)
                    {
                        /** Configuration confirmed. */
                        fscanf(file, "%d// %d// %d//\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                    }
                    else
                    {
                        /** Failed, checking configuration V//VN */
                        newerMatches = fscanf(file, "%d//%d %d//%d %d//%d", x, x, x, x, x, x);
                        fsetpos(file, &start);
                        assertMsg(newerMatches == 6, "Unknown .obj face for .obj file!");
                        /** Configuration confirmed. */
                        newerMatches = fscanf(file, "%d//%d %d//%d %d//%d",
                                              &vertexIndex[0], &normalIndex[0],
                                              &vertexIndex[1], &normalIndex[1],
                                              &vertexIndex[2], &normalIndex[2]);
                    }
                    break;
                }
                break;
                default:
                {
                    assertMsg(true == false, "Unknown faces format in .obj file!");
                    break;
                }
                }

                if (matches == 9 || matches == 2 || matches == 1)
                {
                    o_result->getMaterial(materialsI).setVertexFace(faceI, vertexIndex[0] - 1);
                    o_result->getMaterial(materialsI).setVertexFace(faceI + 1, vertexIndex[1] - 1);
                    o_result->getMaterial(materialsI).setVertexFace(faceI + 2, vertexIndex[2] - 1);
                }
                if (matches == 9 || matches == 2)
                {
                    o_result->getMaterial(materialsI).setSTFace(faceI, coordIndex[0] - 1);
                    o_result->getMaterial(materialsI).setSTFace(faceI + 1, coordIndex[1] - 1);
                    o_result->getMaterial(materialsI).setSTFace(faceI + 2, coordIndex[2] - 1);
                    o_result->getMaterial(materialsI).setSTsPresent(true);
                }

                if (matches == 9)
                {
                    o_result->getMaterial(materialsI).setNormalFace(faceI, normalIndex[0] - 1);
                    o_result->getMaterial(materialsI).setNormalFace(faceI + 1, normalIndex[1] - 1);
                    o_result->getMaterial(materialsI).setNormalFace(faceI + 2, normalIndex[2] - 1);
                    o_result->getMaterial(materialsI).setNormalsPresent(true);
                    faceI += 3;
                }
                else if (matches == 2)
                {
                    faceI += 2;
                }
                else if (matches == 1)
                {
                    if (newerMatches == 3)
                    {
                        faceI += 1;
                    }
                    else if (newerMatches == 6)
                    {
                        o_result->getMaterial(materialsI).setNormalFace(faceI, normalIndex[0] - 1);
                        o_result->getMaterial(materialsI).setNormalFace(faceI + 1, normalIndex[1] - 1);
                        o_result->getMaterial(materialsI).setNormalFace(faceI + 2, normalIndex[2] - 1);
                        o_result->getMaterial(materialsI).setNormalsPresent(true);
                        faceI += 2;
                    }
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
}
