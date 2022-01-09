/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# André Guilherme <andregui17@outlook.com>
*/

#include "../include/loaders/md_loader.hpp"

#include "../include/utils/string.hpp"
#include "../include/utils/debug.hpp"
#include "../include/loaders/anorms.hpp"
#include "../include/models/math/vector3.hpp"
#include "../include/models/math/point.hpp"
#include <string.h>

// ----
// Constructors/Destructors
// ----

MDLoader::MDLoader() {}

MDLoader::~MDLoader() {}

// ----
// Methods
// ----

int MEM_fread(char *buf, size_t size, size_t n, const FILE *f)
{
    memcpy(buf, f, size * n);
    f += size * n;
    return n;
}

/** 
 * Sandro: Possible PS2SDK bug here? 
 * When I wanted to fopen() -> fread() -> allocate frames memory (vertices etc) -> fclose() = I had in about 10x errors in console
 * When I did fopen() -> fread() -> fclose() -> allocate frames memory = I had no errors 🤨
 * It looks like memory allocation during fopen()-fread()-fclose() is broken.
 */
MeshFrame *MDLoader::load_md2(u32 &o_framesCount, char *t_subpath, char *t_nameWithoutExtension, float t_scale, u8 t_invertT)
{
    consoleLog("Loading new MD2 file");
    char *part1 = String::createConcatenated(t_subpath, t_nameWithoutExtension);
    char *part2 = String::createConcatenated("host:", part1);
    char *finalPath = String::createConcatenated(part2, ".md2"); // "folder/object.md2"
    delete[] part1;
    delete[] part2;
    md2_t header;

    FILE *file = fopen(finalPath, "rb");
    assertMsg(file != NULL, "Failed to load .md2 file!");

    fread((char *)&header, sizeof(md2_t), 1, file);

    assertMsg((header.ident == MD2_IDENT) && (header.version == MD2_VERSION), "This MD2 file was not in correct format!");

    u32 framesCount = header.num_frames;
    u32 vertexCount = header.num_xyz;
    u32 stsCount = header.num_st;
    u32 trianglesCount = header.num_tris;

    char framesBuffer[framesCount * header.framesize];
    fseek(file, header.ofs_frames, SEEK_SET);
    fread(&framesBuffer, framesCount * header.framesize, 1, file);

    char stsBuffer[stsCount * sizeof(texCoord_t)];
    fseek(file, header.ofs_st, SEEK_SET);
    fread(&stsBuffer, stsCount * sizeof(texCoord_t), 1, file);

    char trianglesBuffer[trianglesCount * sizeof(triangle_t)];
    fseek(file, header.ofs_tris, SEEK_SET);
    fread(&trianglesBuffer, trianglesCount * sizeof(triangle_t), 1, file);

    fclose(file);

    MeshFrame *resultFrames = new MeshFrame[framesCount];

    frame_t *frame;
    Vector3 tempVec = Vector3();
    for (u32 j = 0; j < framesCount; j++)
    {
        resultFrames[j].allocateVertices(vertexCount);
        resultFrames[j].allocateNormals(vertexCount); // can be optimized
        resultFrames[j].allocateSTs(stsCount);
        resultFrames[j].allocateMaterials(1);
        resultFrames[j].getMaterial(0).allocateFaces(trianglesCount * 3);
        resultFrames[j].getMaterial(0).setName(t_nameWithoutExtension);
        frame = (frame_t *)&framesBuffer[header.framesize * j];
        for (u32 i = 0; i < vertexCount; i++)
        {
            tempVec.set(
                ((frame->verts[i].v[0] * frame->scale[0]) + frame->translate[0]) * t_scale,
                ((frame->verts[i].v[1] * frame->scale[1]) + frame->translate[1]) * t_scale,
                ((frame->verts[i].v[2] * frame->scale[2]) + frame->translate[2]) * t_scale);
            resultFrames[j].setVertex(i, tempVec);

            tempVec.set(
                ANORMS[frame->verts[i].lightnormalindex][0],
                ANORMS[frame->verts[i].lightnormalindex][1],
                ANORMS[frame->verts[i].lightnormalindex][2]);
            resultFrames[j].setNormal(i, tempVec);
        }
    }

    Point tempPoint = Point();
    texCoord_t *texCoord;
    for (u32 i = 0; i < stsCount; i++)
    {
        texCoord = (texCoord_t *)&stsBuffer[sizeof(texCoord_t) * i];
        tempPoint.set(
            (float)texCoord->s / header.skinwidth,
            (float)texCoord->t / header.skinheight);
        if (t_invertT)
            tempPoint.y = 1.0F - tempPoint.y;
        for (u32 j = 0; j < framesCount; j++)
            resultFrames[j].setST(i, tempPoint);
    }

    triangle_t *triangle;
    for (u32 i = 0; i < trianglesCount; i++)
    {
        triangle = (triangle_t *)&trianglesBuffer[sizeof(triangle_t) * i];
        for (u8 j = 0; j < 3; j++)
        {
            for (u32 x = 0; x < framesCount; x++)
            {
                resultFrames[x].getMaterial(0).setSTsPresent(true);
                resultFrames[x].getMaterial(0).setNormalsPresent(true);
                resultFrames[x].getMaterial(0).setVertexFace((i * 3) + j, triangle->index_xyz[j]);
                resultFrames[x].getMaterial(0).setSTFace((i * 3) + j, triangle->index_st[j]);
                resultFrames[x].getMaterial(0).setNormalFace((i * 3) + j, triangle->index_xyz[j]);
            }
        }
    }

    consoleLog("MD2 file loaded!");
    delete[] finalPath;
    o_framesCount = framesCount;
    for (u32 i = 0; i < framesCount; i++)
        resultFrames[i].calculateBoundingBoxes();
    return resultFrames;
}
