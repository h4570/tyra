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
#include "../include/loaders/md_loader.hpp"
#include "../include/loaders/dff_loader.hpp"
#include "../include/loaders/bmp_loader.hpp"
#include "../include/models/texture.hpp"
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
    _areFramesAllocated = false;
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
    setDefaultLODAndClut();
}

Mesh::~Mesh()
{
    if (_areFramesAllocated)
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
    _areFramesAllocated = true;
    char *part1 = String::createConcatenated(t_subfolder, t_objFile); // "folder/object"
    char *finalPath = String::createConcatenated(part1, ".obj");      // "folder/object.obj"
    loader.load(&frames[0], finalPath, t_scale, t_invertT);
    delete[] part1;
    delete[] finalPath;
    _isMother = true;
}

void Mesh::loadObj(char *t_subfolder, char *t_objFile, const float &t_scale, const u32 &t_framesCount, const u8 &t_invertT)
{
    assertMsg(t_framesCount != 0, "Frames count cannot be 0!");
    if (t_framesCount == 1)
        loadObj(t_subfolder, t_objFile, t_scale, t_invertT);
    else
    {
        ObjLoader loader = ObjLoader();
        framesCount = t_framesCount;
        frames = new MeshFrame[framesCount];
        _areFramesAllocated = true;
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
    _areFramesAllocated = true;
    loader.load(frames, dffPath, t_scale, t_invertT);
    delete[] part1;
    delete[] dffPath;
    _isMother = true;
}

void Mesh::loadMD2(char *t_subfolder, char *t_md2File, const float &t_scale, const u8 &t_invertT)
{
    MDLoader loader = MDLoader();
    frames = loader.load_md2(framesCount, t_subfolder, t_md2File, t_scale, t_invertT);
    _isMother = true;
}
/*
todo: fix this error regarding this void undefined reference to `_ZN8MDLoader8load_md3ERjPcS1_fh'
void Mesh::loadMD3(char *t_subfolder, char *t_md3File, const float &t_scale, const u8 &t_invertT) 
{
   MDLoader loader = MDLoader();
   frames = loader.load_md3(framesCount, t_subfolder, t_md3File, t_scale, t_invertT);
   _isMother = true;
}
*/
void Mesh::loadFrom(const Mesh &t_mesh)
{
    framesCount = t_mesh.framesCount;
    frames = new MeshFrame[framesCount];
    _areFramesAllocated = true;
    for (u32 i = 0; i < framesCount; i++)
        frames[i].copyFrom(&t_mesh.getFrame(i));
}

void Mesh::playAnimation(const u32 &t_startFrame, const u32 &t_endFrame)
{
    assertMsg(framesCount > 0, "Cant play animation, because no mesh data was loaded!");
    assertMsg(framesCount != 1, "Cant play animation, because this mesh have only one frame.");
    assertMsg(t_endFrame < framesCount, "End frame value is too high. Valid range: (0, getFramesCount()-1)");
    animState.startFrame = t_startFrame;
    animState.endFrame = t_endFrame;
    if (animState.currentFrame == t_startFrame)
        animState.nextFrame = t_endFrame;
    else
        animState.nextFrame = t_startFrame;
}

void Mesh::playAnimation(const u32 &t_startFrame, const u32 &t_endFrame, const u32 &t_stayFrame)
{
    assertMsg(framesCount > 0, "Cant play animation, because no mesh data was loaded!");
    assertMsg(framesCount != 1, "Cant play animation, because this mesh have only one frame.");
    assertMsg(t_endFrame < framesCount, "End frame value is too high. Valid range: (0, getFramesCount()-1)");
    animState.startFrame = t_startFrame;
    animState.endFrame = t_endFrame;
    animState.isStayFrameSet = true;
    animState.stayFrame = t_stayFrame;
    animState.nextFrame = t_startFrame;
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
    VECTOR ONE_VEC = {1.0F, 1.0F, 1.0F, 1.0F};
    asm volatile(
        // VU0 macro program:
        // Load vector with 1.0F values to VF21
        "lqc2 $vf21, 0x0(%0) \n\t" // load "one vec"
        :
        : "r"(ONE_VEC));

    u32 addedFaces = 0;
#define CURR_FRAME frames[animState.currentFrame]
#define NEXT_FRAME frames[animState.nextFrame]

    MeshMaterial *material = &CURR_FRAME.getMaterial(t_materialIndex); // cache
    u32 *vertFaces = material->getVertexFaces();                       // cache
    u32 *normalFaces = material->getNormalFaces();                     // cache
    u32 *stFaces = material->getSTFaces();                             // cache
    Vector3 *verts = CURR_FRAME.getVertices();                         // cache
    Vector3 *nextVerts = NEXT_FRAME.getVertices();                     // cache
    Point *sts = CURR_FRAME.getSTs();                                  // cache
    Vector3 *normals = CURR_FRAME.getNormals();                        // cache

    for (u32 faceI = 0; faceI < material->getFacesCount(); faceI += 3)
    {
        if (animState.currentFrame != animState.nextFrame)
        {
            asm volatile(
                // VU0 macro program:
                // Calculate lerp() and store data into calc3Vectors

                // Vertex 0
                "lqc2      $vf4, 0x0(%3)  \n\t"   // $vf4 = v1
                "lqc2      $vf5, 0x0(%6)  \n\t"   // $vf5 = v2
                "mfc1      $10,  %9       \n\t"   // $vf6 = t
                "qmtc2     $10,  $vf6      \n\t"  // lerp:
                "vsub.xyz  $vf7, $vf5, $vf4 \n\t" // $vf7 = v2 - v1
                "vmulx.xyz $vf8, $vf7, $vf6 \n\t" // $vf8 = $vf7 * t
                "vadd.xyz  $vf9, $vf8, $vf4 \n\t" // $vf9 = $vf8 + $vf4
                "sqc2      $vf9, 0x0(%0)  \n\t"   // v0  = $vf9

                // Vertex 1
                "lqc2      $vf4, 0x0(%4)  \n\t"   // $vf4 = v1
                "lqc2      $vf5, 0x0(%7)  \n\t"   // $vf5 = v2
                "mfc1      $10,  %9       \n\t"   // $vf6 = t
                "qmtc2     $10,  $vf6      \n\t"  // lerp:
                "vsub.xyz  $vf7, $vf5, $vf4 \n\t" // $vf7 = v2 - v1
                "vmulx.xyz $vf8, $vf7, $vf6 \n\t" // $vf8 = $vf7 * t
                "vadd.xyz  $vf9, $vf8, $vf4 \n\t" // $vf9 = $vf8 + $vf4
                "sqc2      $vf9, 0x0(%1)  \n\t"   // v0  = $vf9

                // Vertex 2
                "lqc2      $vf4, 0x0(%5)  \n\t"   // $vf4 = v1
                "lqc2      $vf5, 0x0(%8)  \n\t"   // $vf5 = v2
                "mfc1      $10,  %9       \n\t"   // $vf6 = t
                "qmtc2     $10,  $vf6      \n\t"  // lerp:
                "vsub.xyz  $vf7, $vf5, $vf4 \n\t" // $vf7 = v2 - v1
                "vmulx.xyz $vf8, $vf7, $vf6 \n\t" // $vf8 = $vf7 * t
                "vadd.xyz  $vf9, $vf8, $vf4 \n\t" // $vf9 = $vf8 + $vf4
                "sqc2      $vf9, 0x0(%2)  \n\t"   // v0  = $vf9

                :
                : "r"(calc3Vectors[0].xyz),
                  "r"(calc3Vectors[1].xyz),
                  "r"(calc3Vectors[2].xyz),
                  "r"(verts[vertFaces[faceI]].xyz),
                  "r"(verts[vertFaces[faceI + 1]].xyz),
                  "r"(verts[vertFaces[faceI + 2]].xyz),
                  "r"(nextVerts[vertFaces[faceI]].xyz),
                  "r"(nextVerts[vertFaces[faceI + 1]].xyz),
                  "r"(nextVerts[vertFaces[faceI + 2]].xyz),
                  "f"(animState.interpolation)
                : "$10");
        }
        else
        {
            asm volatile(
                // VU0 macro program
                // Copy 0,1,2 vertices
                "lqc2 $vf1, 0x0(%3) \n\t" // load vert
                "lqc2 $vf2, 0x0(%4) \n\t" // load normal
                "lqc2 $vf3, 0x0(%5) \n\t" // load st
                "sqc2 $vf1, 0x0(%0) \n\t" // store vert
                "sqc2 $vf2, 0x0(%1) \n\t" // store normal
                "sqc2 $vf3, 0x0(%2) \n\t" // store st
                :
                : "r"(calc3Vectors[0].xyz),
                  "r"(calc3Vectors[1].xyz),
                  "r"(calc3Vectors[2].xyz),
                  "r"(verts[vertFaces[faceI]].xyz),
                  "r"(verts[vertFaces[faceI + 1]].xyz),
                  "r"(verts[vertFaces[faceI + 2]].xyz));
        }

        if (!shouldBeBackfaceCulled ||
            !Vector3::shouldBeBackfaceCulled(&t_cameraPos, &calc3Vectors[2], &calc3Vectors[1], &calc3Vectors[0]))
        {

            asm volatile(
                // VU0 macro program:
                // Copy data and set vert/normal "w" and st "z"+"w" to 1.0F

                // Vertex 0
                "lqc2 $vf1, 0x0(%3) \n\t"         // load vert
                "lqc2 $vf2, 0x0(%4) \n\t"         // load normal
                "lqc2 $vf3, 0x0(%5) \n\t"         // load st
                "vadd.w  $vf1, $vf20, $vf21 \n\t" // set vert.w to 1.0F
                "vadd.w  $vf2, $vf20, $vf21 \n\t" // set normal.W to 1.0F
                "vadd.zw $vf3, $vf20, $vf21 \n\t" // set st.zw to 1.0F
                "sqc2 $vf1, 0x0(%0) \n\t"         // store vert
                "sqc2 $vf2, 0x0(%1) \n\t"         // store normal
                "sqc2 $vf3, 0x0(%2) \n\t"         // store st

                // Vertex 1
                "lqc2 $vf1, 0x0(%9) \n\t"         // load vert
                "lqc2 $vf2, 0x0(%10) \n\t"        // load normal
                "lqc2 $vf3, 0x0(%11) \n\t"        // load st
                "vadd.w  $vf1, $vf20, $vf21 \n\t" // set vert.w to 1.0F
                "vadd.w  $vf2, $vf20, $vf21 \n\t" // set normal.W to 1.0F
                "vadd.zw $vf3, $vf20, $vf21 \n\t" // set st.zw to 1.0F
                "sqc2 $vf1, 0x0(%6) \n\t"         // store vert
                "sqc2 $vf2, 0x0(%7) \n\t"         // store normal
                "sqc2 $vf3, 0x0(%8) \n\t"         // store st

                // Vertex 2
                "lqc2 $vf1, 0x0(%15) \n\t"        // load vert
                "lqc2 $vf2, 0x0(%16) \n\t"        // load normal
                "lqc2 $vf3, 0x0(%17) \n\t"        // load st
                "vadd.w  $vf1, $vf20, $vf21 \n\t" // set vert.w to 1.0F
                "vadd.w  $vf2, $vf20, $vf21 \n\t" // set normal.W to 1.0F
                "vadd.zw $vf3, $vf20, $vf21 \n\t" // set st.zw to 1.0F
                "sqc2 $vf1, 0x0(%12) \n\t"        // store vert
                "sqc2 $vf2, 0x0(%13) \n\t"        // store normal
                "sqc2 $vf3, 0x0(%14) \n\t"        // store st

                :
                : "r"(o_vertices[addedFaces]),
                  "r"(o_normals[addedFaces]),
                  "r"(o_coordinates[addedFaces]),
                  "r"(calc3Vectors[0].xyz),
                  "r"(normals[normalFaces[faceI]].xyz),
                  "r"(sts[stFaces[faceI]].xy),
                  "r"(o_vertices[addedFaces + 1]),
                  "r"(o_normals[addedFaces + 1]),
                  "r"(o_coordinates[addedFaces + 1]),
                  "r"(calc3Vectors[1].xyz),
                  "r"(normals[normalFaces[faceI + 1]].xyz),
                  "r"(sts[stFaces[faceI + 1]].xy),
                  "r"(o_vertices[addedFaces + 2]),
                  "r"(o_normals[addedFaces + 2]),
                  "r"(o_coordinates[addedFaces + 2]),
                  "r"(calc3Vectors[2].xyz),
                  "r"(normals[normalFaces[faceI + 2]].xyz),
                  "r"(sts[stFaces[faceI + 2]].xy));
            addedFaces += 3;
        }
    }
    return addedFaces;
}

u8 Mesh::isInFrustum(Plane *t_frustumPlanes)
{
    Vector3 boxCalcTemp;
    u8 boxResult = 1, boxIn = 0, boxOut = 0;
    const Vector3 *currentBoundingBox = getCurrentBoundingBoxVertices();
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
