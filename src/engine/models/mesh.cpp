/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/models/mesh.hpp"
#include "../include/loaders/obj_loader.hpp"
#include "../include/loaders/md2_loader.hpp"
#include "../include/loaders/dff_loader.hpp"
#include "../include/loaders/bmp_loader.hpp"
#include "../include/models/mesh_texture.hpp"
#include "../include/utils/debug.hpp"
#include "../include/utils/string.hpp"
#include <cstdlib>

// ----
// Constructors/Destructors
// ----

Mesh::Mesh()
{
    id = rand() % 1000000;
    shouldBeFrustumCulled = true;
    shouldBeBackfaceCulled = false;
    shouldBeLighted = false;
    _isMother = false;
    scale = 1.0F;
    framesCount = 0;
    animState.startFrame = 0;
    animState.endFrame = 0;
    animState.interpolation = 0.0F;
    animState.animType = 0;
    animState.currentFrame = 0;
    animState.stayFrame = 0;
    animState.isStayFrameSet = false;
    animState.nextFrame = 0;
    animState.speed = 0.1F;
    setDefaultColor();
    setDefaultLODAndClut();
}

Mesh::~Mesh()
{
    if (_isMother)
        delete[] frames;
}

// ----
// Methods
// ----

void Mesh::loadObj(char *t_subfolder, char *t_objFile, const float &t_scale, const u8 &t_invertT)
{
    ObjLoader loader = ObjLoader();
    framesCount = 1;
    frames = new MeshFrame[framesCount];
    char *part1 = String::createConcatenated(t_subfolder, t_objFile); // "folder/object"
    char *finalPath = String::createConcatenated(part1, ".obj");      // "folder/object.obj"
    loader.load(&frames[0], finalPath, t_scale, t_invertT);
    delete[] part1;
    delete[] finalPath;
    _isMother = true;
}

void Mesh::loadObj(char *t_subfolder, char *t_objFile, const float &t_scale, const u32 &t_framesCount, const u8 &t_invertT)
{
    if (t_framesCount == 0)
        PRINT_ERR("Frames count cannot be 0!");
    else if (t_framesCount == 1)
        loadObj(t_subfolder, t_objFile, t_scale, t_invertT);
    else
    {
        ObjLoader loader = ObjLoader();
        framesCount = t_framesCount;
        frames = new MeshFrame[framesCount];
        char *part1 = String::createConcatenated(t_subfolder, t_objFile); // "folder/object"
        char *part2 = String::createConcatenated(part1, "_");             // "folder/object_"
        for (u32 i = 0; i < framesCount; i++)
        {
            char *part3 = String::createU32ToString(i + 1);              // 0 -> "1"
            char *part4 = String::createWithLeadingZeros(part3);         // "000001"
            char *part5 = String::createConcatenated(part2, part4);      // "folder/object_000001"
            char *finalPath = String::createConcatenated(part5, ".obj"); // "folder/object_000001.obj"
            loader.load(&frames[i], finalPath, t_scale, t_invertT);
            delete[] part3;
            delete[] part4;
            delete[] part5;
            delete[] finalPath;
        }
        delete[] part2;
        delete[] part1;
        _isMother = true;
    }
}

void Mesh::loadDff(char *t_subfolder, char *t_dffFile, const float &t_scale, const u8 &t_invertT)
{
    DffLoader loader = DffLoader();
    char *part1 = String::createConcatenated(t_subfolder, t_dffFile);
    char *dffPath = String::createConcatenated(part1, ".dff");
    framesCount = 1;
    frames = new MeshFrame[1];
    loader.load(frames, dffPath, t_scale, t_invertT);
    delete[] part1;
    delete[] dffPath;
    _isMother = true;
}

void Mesh::loadMD2(char *t_subfolder, char *t_md2File, const float &t_scale, const u8 &t_invertT)
{
    MD2Loader loader = MD2Loader();
    frames = loader.load(framesCount, t_subfolder, t_md2File, t_scale, t_invertT);
    _isMother = true;
}

void Mesh::loadFrom(const Mesh &t_mesh)
{
    frames = t_mesh.frames;
    framesCount = t_mesh.framesCount;
}

void Mesh::playAnimation(const u32 &t_startFrame, const u32 &t_endFrame)
{
    if (framesCount > 1)
    {
        if (t_endFrame >= framesCount)
            PRINT_ERR("End frame value is too high. Valid range: (0, getFramesCount()-1)");
        animState.startFrame = t_startFrame;
        animState.endFrame = t_endFrame;
        if (animState.currentFrame == t_startFrame)
            animState.nextFrame = t_endFrame;
        else
            animState.nextFrame = t_startFrame;
    }
    else if (framesCount == 0)
        PRINT_ERR("Cant play animation, because no mesh data was loaded!");
    else if (framesCount == 1)
        PRINT_ERR("Cant play animation, because this mesh have only one frame.");
}

void Mesh::playAnimation(const u32 &t_startFrame, const u32 &t_endFrame, const u32 &t_stayFrame)
{
    if (framesCount > 1)
    {
        if (t_endFrame >= framesCount)
            PRINT_ERR("End frame value is too high. Valid range: (0, getFramesCount()-1)");
        animState.startFrame = t_startFrame;
        animState.endFrame = t_endFrame;
        animState.isStayFrameSet = true;
        animState.stayFrame = t_stayFrame;
        animState.nextFrame = t_startFrame;
    }
    else if (framesCount == 0)
        PRINT_ERR("Cant play animation, because no mesh data was loaded!");
    else if (framesCount == 1)
        PRINT_ERR("Cant play animation, because this mesh have only one frame.");
}

void Mesh::animate()
{
    animState.interpolation += animState.speed;
    if (animState.interpolation >= 1.0F)
    {
        animState.interpolation = 0.0F;
        animState.currentFrame = animState.nextFrame;
        if (++animState.nextFrame > animState.endFrame)
        {
            if (animState.isStayFrameSet)
            {
                animState.isStayFrameSet = false;
                animState.nextFrame = animState.stayFrame;
                animState.startFrame = animState.stayFrame;
                animState.endFrame = animState.stayFrame;
            }
            else
                animState.nextFrame = animState.startFrame;
        }
    }
}

u32 Mesh::getDrawData(u32 t_materialIndex, VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, Vector3 &t_cameraPos)
{
    u32 addedFaces = 0;
#define CURR_FRAME frames[animState.currentFrame]
#define NEXT_FRAME frames[animState.nextFrame]
    MeshMaterial *material = &CURR_FRAME.getMaterial(t_materialIndex); // cache
    Vector3 *verts = CURR_FRAME.getVertices();                         // cache
    Vector3 *nextVerts = NEXT_FRAME.getVertices();                     // cache
    Point *sts = CURR_FRAME.getSTs();                                  // cache
    Vector3 *normals = CURR_FRAME.getNormals();                        // cache
#define CURR_VERT verts[material->getVertexFace(matI + vertI)]
#define CURR_ST sts[material->getSTFace(matI + vertI)]
#define CURR_NORMAL normals[material->getNormalFace(matI + vertI)]
#define NEXT_VERT nextVerts[material->getVertexFace(matI + vertI)]

    for (u32 matI = 0; matI < material->getFacesCount(); matI += 3)
    {
        for (u32 vertI = 0; vertI < 3; vertI++)
            if (animState.currentFrame != animState.nextFrame)
                calc3Vectors[vertI].setByLerp(CURR_VERT, NEXT_VERT, animState.interpolation, scale);

        if (!shouldBeBackfaceCulled ||
            Vector3::shouldBeBackfaceCulled(&t_cameraPos, &calc3Vectors[0], &calc3Vectors[1], &calc3Vectors[2]))

            for (u32 vertI = 0; vertI < 3; vertI++)
            {

                if (animState.currentFrame == animState.nextFrame)
                {
                    o_vertices[addedFaces][0] = CURR_VERT.x;
                    o_vertices[addedFaces][1] = CURR_VERT.y;
                    o_vertices[addedFaces][2] = CURR_VERT.z;
                }
                else
                {
                    o_vertices[addedFaces][0] = calc3Vectors[vertI].x;
                    o_vertices[addedFaces][1] = calc3Vectors[vertI].y;
                    o_vertices[addedFaces][2] = calc3Vectors[vertI].z;
                }
                o_vertices[addedFaces][3] = 1.0F;

                o_normals[addedFaces][0] = CURR_NORMAL.x;
                o_normals[addedFaces][1] = CURR_NORMAL.y;
                o_normals[addedFaces][2] = CURR_NORMAL.z;
                o_normals[addedFaces][3] = 1.0F;

                o_coordinates[addedFaces][0] = CURR_ST.x;
                o_coordinates[addedFaces][1] = CURR_ST.y;
                o_coordinates[addedFaces][2] = 1.0F;
                o_coordinates[addedFaces++][3] = 1.0F;
            }
    }
    return addedFaces;
}

u8 Mesh::isInFrustum(Plane *t_frustumPlanes)
{
    Vector3 boxCalcTemp;
    u8 boxResult = 1, boxIn = 0, boxOut = 0;
    Vector3 *currentBoundingBox = getCurrentBoundingBox();
    for (int i = 0; i < 6; i++)
    {
        boxOut = 0;
        boxIn = 0;
        // for each corner of the box do ...
        // get out of the cycle as soon as a box as corners
        // both inside and out of the frustum
        for (int y = 0; y < 8 && (boxIn == 0 || boxOut == 0); y++)
        {
            boxCalcTemp.set(
                currentBoundingBox[y].x + position.x,
                currentBoundingBox[y].y + position.y,
                currentBoundingBox[y].z + position.z);
            if (t_frustumPlanes[i].distanceTo(boxCalcTemp) < 0)
                boxOut++;
            else
                boxIn++;
        }
        //if all corners are out
        if (boxIn == 0)
            return 0;
        else if (boxOut)
            boxResult = 1;
    }
    return boxResult;
}

/** Set's default object color + no transparency */
void Mesh::setDefaultColor()
{
    color.r = 0x80;
    color.g = 0x80;
    color.b = 0x80;
    color.a = 0x80;
    color.q = 1.0F;
}

/** Sets texture level of details settings and CLUT settings */
void Mesh::setDefaultLODAndClut()
{
    lod.calculation = LOD_USE_K;
    lod.max_level = 0;
    lod.mag_filter = LOD_MAG_NEAREST;
    lod.min_filter = LOD_MIN_NEAREST;
    lod.l = 0;
    lod.k = 0.0F;

    clut.storage_mode = CLUT_STORAGE_MODE1;
    clut.start = 0;
    clut.psm = 0;
    clut.load_method = CLUT_NO_LOAD;
    clut.address = 0;
}
