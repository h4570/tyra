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

#include "../include/utils/debug.hpp"
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
void MD2Loader::load(MD2Model *o_result, char *t_filename, float t_scale)
{
    PRINT_LOG("Loading new MD2 file");
    md2_t header; // md2 header
    char *buffer; // buffer storing frame data

    FILE *file = fopen(t_filename, "rb");

    if (file == NULL)
        PRINT_ERR("Failed to load .md2 file!");

    fread((char *)&header, sizeof(md2_t), 1, file);

    if ((header.ident != MD2_IDENT) && (header.version != MD2_VERSION))
        PRINT_ERR("This MD2 file was not in correct format!");

    o_result->framesCount = header.num_frames;
    o_result->verticesPerFrameCount = header.num_xyz;
    o_result->coordinatesCount = header.num_st;
    o_result->trianglesCount = header.num_tris;

    printf("MD2 - Frames: %d\n", header.num_frames);
    printf("MD2 - Vert: %d\n", header.num_xyz);
    printf("MD2 - Tris: %d\n", header.num_tris);
    printf("MD2 - STs: %d\n", header.num_st);
    printf("MD2 - Skin W:%d H: %d\n", header.skinwidth, header.skinheight);

    o_result->allocateMemory();

    buffer = new char[o_result->framesCount * header.framesize];
    fseek(file, header.ofs_frames, SEEK_SET);
    fread((char *)buffer, o_result->framesCount * header.framesize, 1, file);

    frame_t *frame; // temporary vars
    Vector3 *ptrVerts;
    u32 *ptrNormals;
    // vertex array initialization
    for (u32 j = 0; j < o_result->framesCount; j++)
    {
        frame = (frame_t *)&buffer[header.framesize * j];
        ptrVerts = &o_result->vertices[o_result->verticesPerFrameCount * j];
        ptrNormals = &o_result->normalIndexes[o_result->verticesPerFrameCount * j];

        for (u32 i = 0; i < o_result->verticesPerFrameCount; i++)
        {
            ptrVerts[i].x = ((frame->verts[i].v[0] * frame->scale[0]) + frame->translate[0]) * t_scale;
            ptrVerts[i].y = ((frame->verts[i].v[1] * frame->scale[1]) + frame->translate[1]) * t_scale;
            ptrVerts[i].z = ((frame->verts[i].v[2] * frame->scale[2]) + frame->translate[2]) * t_scale;

            ptrNormals[i] = frame->verts[i].lightnormalindex;
        }
        if (j == o_result->framesCount - 1)
            printf("Last frame, last vert: X:%f Y:%f Z:%f\n",
                   ptrVerts[o_result->verticesPerFrameCount - 1].x,
                   ptrVerts[o_result->verticesPerFrameCount - 1].y,
                   ptrVerts[o_result->verticesPerFrameCount - 1].z);
    }
    delete[] buffer;

    buffer = new char[o_result->coordinatesCount * sizeof(texCoord_t)];
    fseek(file, header.ofs_st, SEEK_SET);
    fread((char *)buffer, o_result->coordinatesCount * sizeof(texCoord_t), 1, file);

    texCoord_t *texCoord;
    for (u32 i = 0; i < o_result->coordinatesCount; i++)
    {
        texCoord = (texCoord_t *)&buffer[sizeof(texCoord_t) * i];
        o_result->coordinates[i].x = (float)texCoord->s / header.skinwidth;
        o_result->coordinates[i].y = (float)texCoord->t / header.skinheight;
    }
    delete[] buffer;

    printf("Last tex coord(%d): S:%f T:%f\n",
           o_result->coordinatesCount,
           o_result->coordinates[o_result->coordinatesCount - 1].x,
           o_result->coordinates[o_result->coordinatesCount - 1].y);

    buffer = new char[o_result->trianglesCount * sizeof(triangle_t)];
    fseek(file, header.ofs_tris, SEEK_SET);
    fread((char *)buffer, o_result->trianglesCount * sizeof(triangle_t), 1, file);

    triangle_t *triangle; // temporary var
    // vertex array initialization
    for (u32 i = 0; i < o_result->trianglesCount; i++)
    {
        triangle = (triangle_t *)&buffer[sizeof(triangle_t) * i];
        for (u8 j = 0; j < 3; j++)
        {
            o_result->triangles[i].verticeIndexes[j] = triangle->index_xyz[j];
            o_result->triangles[i].coordIndexes[j] = triangle->index_st[j];
        }
    }
    delete[] buffer;

    fclose(file);
    PRINT_LOG("MD2 file loaded!");
}
