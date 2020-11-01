/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/loaders/md2_loader.hpp"

#include "../include/utils/string.hpp"
#include "../include/utils/debug.hpp"
#include "../include/loaders/anorms.hpp"
#include "../include/models/math/vector3.hpp"
#include "../include/models/math/point.hpp"
#include <string.h>
#include <iosfwd>

// ----
// Constructors/Destructors
// ----

MD2Loader::MD2Loader() {}

MD2Loader::~MD2Loader() {}

// ----
// Methods
// ----

/** Load, parse .obj file and load data into given MeshSpec */
u32 MD2Loader::load(MeshFrame *o_result, char *t_subpath, char *t_nameWithoutExtension, float t_scale, u8 t_invertT)
{
    PRINT_LOG("Loading new MD2 file");
    char *part1 = String::createConcatenated(t_subpath, t_nameWithoutExtension);
    char *finalPath = String::createConcatenated(part1, ".md2"); // "folder/object.md2"
    delete[] part1;

    PRINT_LOG("Loading new MD2 file");
    md2_t header; // md2 header

    char *buffer; // buffer storing frame data

    FILE *file = fopen(finalPath, "rb");

    if (file == NULL)
    {
        PRINT_ERR("Failed to load .md2 file!");
        return 0;
    }

    fread((char *)&header, sizeof(md2_t), 1, file);

    if ((header.ident != MD2_IDENT) && (header.version != MD2_VERSION))
    {
        PRINT_ERR("This MD2 file was not in correct format!");
        return 0;
    }

    u32 framesCount = header.num_frames;
    u32 vertexCount = header.num_xyz;
    u32 stsCount = header.num_st;
    u32 trianglesCount = header.num_tris;

    // o_result = new MeshFrame[framesCount];

    buffer = new char[framesCount * header.framesize];
    fseek(file, header.ofs_frames, SEEK_SET);
    fread((char *)buffer, framesCount * header.framesize, 1, file);

    frame_t *frame; // temporary vars
    Vector3 tempVec = Vector3();

    for (u32 j = 0; j < framesCount; j++)
    {
        o_result[j].allocateVertices(vertexCount);
        o_result[j].allocateNormals(vertexCount); // can be optimized
        o_result[j].allocateSTs(stsCount);
        o_result[j].allocateMaterials(1);
        printf("Materials count:%d\n", o_result[j].getMaterialsCount());
        o_result[j].getMaterial(0).allocateFaces(trianglesCount * 3);
        o_result[j].getMaterial(0).setName(t_nameWithoutExtension);

        frame = (frame_t *)&buffer[header.framesize * j];

        for (u32 i = 0; i < vertexCount; i++)
        {
            tempVec.set(
                ((frame->verts[i].v[0] * frame->scale[0]) + frame->translate[0]) * t_scale,
                ((frame->verts[i].v[1] * frame->scale[1]) + frame->translate[1]) * t_scale,
                ((frame->verts[i].v[2] * frame->scale[2]) + frame->translate[2]) * t_scale);
            o_result[j].setVertex(i, tempVec);

            tempVec.set(
                ANORMS[frame->verts[i].lightnormalindex][0],
                ANORMS[frame->verts[i].lightnormalindex][1],
                ANORMS[frame->verts[i].lightnormalindex][2]);
            o_result[j].setNormal(i, tempVec);
        }
    }
    delete[] buffer;

    buffer = new char[stsCount * sizeof(texCoord_t)];
    fseek(file, header.ofs_st, SEEK_SET);
    fread((char *)buffer, stsCount * sizeof(texCoord_t), 1, file);

    Point tempPoint = Point();
    texCoord_t *texCoord;
    for (u32 i = 0; i < stsCount; i++)
    {
        texCoord = (texCoord_t *)&buffer[sizeof(texCoord_t) * i];
        tempPoint.set(
            (float)texCoord->s / header.skinwidth,
            (float)texCoord->t / header.skinheight);
        if (t_invertT)
            tempPoint.y = 1.0F - tempPoint.y;
        for (u32 j = 0; j < framesCount; j++)
            o_result[j].setST(i, tempPoint);
    }
    delete[] buffer;

    buffer = new char[trianglesCount * sizeof(triangle_t)];
    fseek(file, header.ofs_tris, SEEK_SET);
    fread((char *)buffer, trianglesCount * sizeof(triangle_t), 1, file);

    triangle_t *triangle; // temporary var
    // vertex array initialization
    for (u32 i = 0; i < trianglesCount; i++)
    {
        triangle = (triangle_t *)&buffer[sizeof(triangle_t) * i];
        for (u8 j = 0; j < 3; j++)
        {
            for (u32 x = 0; x < framesCount; x++)
            {
                o_result[x].getMaterial(0).setVertexFace((i * 3) + j, triangle->index_xyz[j]);
                o_result[x].getMaterial(0).setSTFace((i * 3) + j, triangle->index_st[j]);
                o_result[x].getMaterial(0).setNormalFace((i * 3) + j, triangle->index_xyz[j]);
            }
        }
    }
    delete[] buffer;

    fclose(file);
    PRINT_LOG("MD2 file loaded!");

    delete[] finalPath;
    return framesCount;
}
