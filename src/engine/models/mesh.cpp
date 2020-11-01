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
    shouldBeFrustumCulled = true;
    shouldBeBackfaceCulled = false;
    shouldBeLighted = false;
    isMd2Loaded = false;
    isObjLoaded = false;
    isDffLoaded = false;
    isSpecInitialized = false;
    id = rand() % 1000000;
    scale = 1.0F;
    animState.startFrame = 0;
    animState.endFrame = 0;
    animState.interpolation = 0.0F;
    animState.animType = 0;
    animState.currentFrame = 0;
    animState.nextFrame = 0;
    animState.speed = 0.1F;
    // filename = String::createWithoutExtension(t_objFile);
}

Mesh::~Mesh()
{
    // TODO
    if (isMd2Loaded)
        delete md2;
    if (isDffLoaded)
        delete dff;
}

// ----
// Methods
// ----

u32 Mesh::getFacesCount()
{
    u32 result = 0; // TODO
    for (u16 i = 0; i < frames[0].getMaterialsCount(); i++)
        result += frames[0].getMaterial(i).getFacesCount();
    return result;
}

void Mesh::animate()
{
    animState.interpolation += animState.speed;
    if (animState.interpolation >= 1.0F)
    {
        animState.interpolation = 0.0F;
        animState.currentFrame = animState.nextFrame;
        if (++animState.nextFrame > animState.endFrame)
            animState.nextFrame = animState.startFrame;
    }
}

u32 Mesh::getDrawData(u32 t_materialIndex, VECTOR *o_vertices, VECTOR *o_normals, VECTOR *o_coordinates, Vector3 &t_cameraPos, float t_scale, u8 t_shouldBeBackfaceCulled)
{
#define CURR_FRAME frames[animState.currentFrame]
#define NEXT_FRAME frames[animState.nextFrame]
#define MATERIAL CURR_FRAME.getMaterial(t_materialIndex)
#define CURR_VERT CURR_FRAME.getVertex(MATERIAL.getVertexFace(matI + vertI))
#define NEXT_VERT NEXT_FRAME.getVertex(MATERIAL.getVertexFace(matI + vertI))
    u32 addedFaces = 0;

    for (u32 matI = 0; matI < MATERIAL.getFacesCount(); matI += 3)
    {
        for (u32 vertI = 0; vertI < 3; vertI++)
            if (animState.startFrame == animState.endFrame)
                calc3Vectors[vertI].set(CURR_VERT * t_scale);
            else
                calc3Vectors[vertI].setByLerp(CURR_VERT, NEXT_VERT, animState.interpolation, t_scale);

        if (!t_shouldBeBackfaceCulled ||
            Vector3::shouldBeBackfaceCulled(&t_cameraPos, &calc3Vectors[2], &calc3Vectors[1], &calc3Vectors[0]))

            for (u32 vertI = 0; vertI < 3; vertI++)
            {
                o_vertices[addedFaces][0] = calc3Vectors[vertI].x;
                o_vertices[addedFaces][1] = calc3Vectors[vertI].y;
                o_vertices[addedFaces][2] = calc3Vectors[vertI].z;
                o_vertices[addedFaces][3] = 1.0F;

                o_normals[addedFaces][0] = CURR_FRAME.getNormal(MATERIAL.getNormalFace(matI + vertI)).x;
                o_normals[addedFaces][1] = CURR_FRAME.getNormal(MATERIAL.getNormalFace(matI + vertI)).y;
                o_normals[addedFaces][2] = CURR_FRAME.getNormal(MATERIAL.getNormalFace(matI + vertI)).z;
                o_normals[addedFaces][3] = 1.0F;

                o_coordinates[addedFaces][0] = CURR_FRAME.getST(MATERIAL.getSTFace(matI + vertI)).x;
                o_coordinates[addedFaces][1] = CURR_FRAME.getST(MATERIAL.getSTFace(matI + vertI)).y;
                o_coordinates[addedFaces][2] = 1.0F;
                o_coordinates[addedFaces++][3] = 1.0F;
            }
    }
    return addedFaces;
}

void Mesh::loadDff(char *t_subfolder, char *t_dffFile, Vector3 &t_initPos, float t_scale)
{
    createSpecIfNotCreated();
    DffLoader loader = DffLoader();
    dff = new DffModel();
    char *dffPath = String::createConcatenated(t_subfolder, t_dffFile);
    loader.load(dff->clump, dffPath, t_scale);
    delete[] dffPath;
    position = t_initPos;
    setVerticesReference(
        dff->clump.geometryList.geometries[0].data.dataHeader.vertexCount,
        dff->clump.geometryList.geometries[0].data.vertexInformation);
    setDefaultColor();
    isDffLoaded = true;
    loadTextures(t_subfolder, ".bmp");
}

void Mesh::setDff(Vector3 &t_initPos, DffModel *t_dffModel)
{
    position = t_initPos;
    isSpecInitialized = 1;
    dff = t_dffModel;
    setVerticesReference(
        dff->clump.geometryList.geometries[0].data.dataHeader.vertexCount,
        dff->clump.geometryList.geometries[0].data.vertexInformation);
    setDefaultColor();
    isDffLoaded = true;
}

void Mesh::loadObj(char *t_subfolder, char *t_objFile, Vector3 &t_initPos, float t_scale, u16 t_framesCount)
{
    createSpecIfNotCreated();
    ObjLoader loader = ObjLoader();

    framesCount = t_framesCount;

    frames = new MeshFrame[framesCount];

    char *part1 = String::createConcatenated(t_subfolder, t_objFile); // "folder/object"
    char *part2 = String::createConcatenated(part1, "_");             // "folder/object_"
    delete[] part1;

    for (u16 i = 0; i < framesCount; i++)
    {
        char *part3 = String::createU32ToString(i + 1);              // 0 -> "1"
        char *part4 = String::createWithLeadingZeros(part3);         // "000001"
        char *part5 = String::createConcatenated(part2, part4);      // "folder/object_000001"
        char *finalPath = String::createConcatenated(part5, ".obj"); // "folder/object_000001.obj"
        loader.load(&frames[i], finalPath, t_scale, false);
        delete[] part3;
        delete[] part4;
        delete[] part5;
        delete[] finalPath;
    }

    delete[] part2;

    position = t_initPos;
    setVerticesReference(getFacesCount(), frames[0].getVertices());
    setDefaultColor();
    isObjLoaded = true;
}

// void Mesh::setObj(Vector3 &t_initPos, ObjModel *t_objModel)
// {
//     position = t_initPos;
//     isSpecInitialized = true;
//     obj = t_objModel;
//     setVerticesReference(getFacesCount(), frames[0].getVertices());
//     setDefaultColor();
//     isObjLoaded = true;
// }

void Mesh::createSpecIfNotCreated()
{
}

void Mesh::setAnimSpeed(float t_value)
{
    if (isMd2Loaded == 1)
        md2->animState.speed = t_value;
    else
        PRINT_ERR("Animation speed set is not possible, because no md2 3D model was loaded!");
}

void Mesh::setVerticesReference(u32 t_verticesCount, Vector3 *t_verticesRef)
{
    verticesCount = t_verticesCount;
    vertices = t_verticesRef;
}

u32 Mesh::getVertexCount()
{
    if (isMd2Loaded)
        return md2->trianglesCount * 3;
    else if (isObjLoaded)
        return getFacesCount();
    else if (isDffLoaded)
        return dff->clump.geometryList.geometries[0].data.dataHeader.triangleCount * 3;
    else
    {
        PRINT_ERR("Can't get vertex count, because no 3D model was loaded!");
        return 0;
    }
}

void Mesh::setDefaultWrapSettings(texwrap_t &t_wrapSettings)
{
    // t_wrapSettings.horizontal = WRAP_REPEAT;
    // t_wrapSettings.vertical = WRAP_REPEAT;
    // t_wrapSettings.maxu = 0;
    // t_wrapSettings.maxv = 0;
    // t_wrapSettings.minu = 0;
    // t_wrapSettings.minv = 0;
}

void Mesh::loadTextures(char *t_subfolder, char *t_extension)
{
    BmpLoader bmpLoader = BmpLoader();
    if (isObjLoaded)
    {
    }
    else if (isMd2Loaded)
    {
        textures = new MeshTexture[1];
        bmpLoader.load(textures[0], t_subfolder, md2->filename, t_extension);
        // setDefaultWrapSettings(spec->textures[0].wrapSettings);
    }
    else if (isDffLoaded)
    {
        textures = new MeshTexture[dff->clump.geometryList.geometries[0].materialList.data.materialCount];
        for (u8 i = 0; i < dff->clump.geometryList.geometries[0].materialList.data.materialCount; i++)
            for (u8 j = 0; j < dff->clump.geometryList.geometries[0].materialList.materials[i].data.textureCount; j++)
            {
                bmpLoader.load(textures[i], t_subfolder, dff->clump.geometryList.geometries[0].materialList.materials[i].textures[j].textureName.text, t_extension);
                // setDefaultWrapSettings(spec->textures[i].wrapSettings);
            }
    }
    else
        PRINT_ERR("Can't load textures, because no 3D model was loaded!");
}

// TODO refactor
u32 Mesh::getDrawData(u32 splitIndex, VECTOR *t_vertices, VECTOR *t_normals, VECTOR *t_coordinates, Vector3 &t_cameraPos)
{
    if (isMd2Loaded)
        return md2->getCurrentFrameData(t_vertices, t_normals, t_coordinates, t_cameraPos, scale, shouldBeBackfaceCulled);
    else if (isObjLoaded)
        return getDrawData(splitIndex, t_vertices, t_normals, t_coordinates, t_cameraPos, scale, shouldBeBackfaceCulled);
    else if (isDffLoaded)
        return dff->getDrawData(splitIndex, t_vertices, t_normals, t_coordinates, t_cameraPos, scale, shouldBeBackfaceCulled);
    PRINT_ERR("Can't get draw data, because no 3D model was loaded!");
    return 0;
}

/** Set object position and specification */
void Mesh::loadMD2(char *t_subfolder, char *t_md2File, Vector3 &t_initPos, float t_scale)
{
    createSpecIfNotCreated();
    md2 = new MD2Model(t_md2File);
    MD2Loader loader = MD2Loader();
    position = t_initPos;
    char *md2Path = String::createConcatenated(t_subfolder, t_md2File);
    loader.load(md2, md2Path, t_scale);
    delete[] md2Path;
    setVerticesReference(md2->verticesPerFrameCount * md2->framesCount, md2->vertices);
    setDefaultColor();
    isMd2Loaded = true;
    loadTextures(t_subfolder, ".bmp");
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

/** Calculates minimum and maximum X, Y, Z of this 3D objects vertices + current position */
void Mesh::getMinMax(Vector3 *t_min, Vector3 *t_max)
{
    Vector3 calc = Vector3();
    u8 isInitialized = 0;
    for (u32 i = 0; i < 8; i++)
    {
        getFarthestVertex(&calc, i);
        if (isInitialized == 0)
        {
            isInitialized = 1;
            t_min->set(calc);
            t_max->set(calc);
        }

        if (t_min->x > calc.x)
            t_min->x = calc.x;
        if (calc.x > t_max->x)
            t_max->x = calc.x;

        if (t_min->y > calc.y)
            t_min->y = calc.y;
        if (calc.y > t_max->y)
            t_max->y = calc.y;

        if (t_min->z > calc.z)
            t_min->z = calc.z;
        if (calc.z > t_max->z)
            t_max->z = calc.z;
    }
}

void Mesh::playAnimation(u32 t_startFrame, u32 t_endFrame)
{
    if (isMd2Loaded == 1)
    {
        md2->animState.startFrame = t_startFrame;
        md2->animState.endFrame = t_endFrame;
    }
    else if (isObjLoaded == 1)
    {
        animState.startFrame = t_startFrame;
        animState.endFrame = t_endFrame;
    }
    else
        PRINT_ERR("Animation is only supported in MD2/OBJ format!");
}

/** Returns next farthest vertex of 3D object
 * @param offset Max 8 (because, box have 8 corners)
 */
void Mesh::getFarthestVertex(Vector3 *o_result, int t_offset)
{
    // TODO current frame
    o_result->x = frames[animState.currentFrame].getBoundingBox(t_offset).x + position.x;
    o_result->y = frames[animState.currentFrame].getBoundingBox(t_offset).y + position.y;
    o_result->z = frames[animState.currentFrame].getBoundingBox(t_offset).z + position.z;
}

/** Sets texture level of details settings and CLUT settings */
void Mesh::setupLodAndClut()
{
    PRINT_LOG("Setting LOD, CLUT");
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
    PRINT_LOG("LOD, CLUT set!");
}

/** Check if box is visible in view frustum */
u8 Mesh::isInFrustum(Plane *t_frustumPlanes)
{
    Vector3 boxCalcTemp;
    int boxResult = 1, boxIn = 0, boxOut = 0;

    for (int i = 0; i < 6; i++)
    {
        boxOut = 0;
        boxIn = 0;
        // for each corner of the box do ...
        // get out of the cycle as soon as a box as corners
        // both inside and out of the frustum
        for (int y = 0; y < 8 && (boxIn == 0 || boxOut == 0); y++)
        {
            getFarthestVertex(&boxCalcTemp, y);
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
