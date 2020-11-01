/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

/*		
*       NOTICE: Based on
*       https://github.com/mrqo/rw_parse/blob/master/rw_utils/rw_utils.cpp
*       rw_utils.cpp	Written by Marek Iwaniuk (c).
*		Dff structure documentation http://www.chronetal.co.uk/gta/index.php?page=dff
*		Created on 02/04/2017 (MM/DD/YYYY)
*/

#include "../include/loaders/dff_loader.hpp"
#include "../include/utils/debug.hpp"
#include "../include/utils/math.hpp"
#include "../include/utils/string.hpp"
#include <cstring>

// ----
// Constructors/Destructors
// ----

DffLoader::DffLoader() {}

DffLoader::~DffLoader() {}

// ----
// Methods
// ----

void DffLoader::load(MeshFrame *o_result, char *t_filename, float t_scale, u8 t_invertT)
{
    PRINT_LOG("Loading dff file");
    FILE *file = fopen(t_filename, "rb");
    if (file == NULL)
        PRINT_ERR("Failed to load .obj file!");
    fseek(file, 0L, SEEK_END);
    long fileSize = ftell(file);
    u8 *data = new u8[fileSize];
    rewind(file);
    fread(data, sizeof(u8), fileSize, file);
    fclose(file);
    RwClump *clump = new RwClump();
    serialize(*clump, data, t_scale);
    delete[] data;
    // delete[] clump; // TODO
    convert(o_result, *clump, t_invertT);
    PRINT_LOG("Dff file loaded!");
}

void DffLoader::convert(MeshFrame *frames, RwClump &t_clump, u8 t_invertT)
{
#define GEOMETRY t_clump.geometryList.geometries[0]

    MeshFrame *frame = &frames[0];
    u32 vertNormalStCount = GEOMETRY.data.dataHeader.vertexCount;
    frame->allocateVertices(vertNormalStCount);
    frame->allocateNormals(vertNormalStCount); // can be optimized
    frame->allocateSTs(vertNormalStCount);     // can be optimized

    Vector3 tempVec = Vector3();
    Point tempPoint = Point();
    for (u32 i = 0; i < vertNormalStCount; i++)
    {
        tempVec.set(
            GEOMETRY.data.vertexInformation[i].x,
            GEOMETRY.data.vertexInformation[i].y,
            GEOMETRY.data.vertexInformation[i].z);
        frame->setVertex(i, tempVec);
        tempVec.set(
            GEOMETRY.data.normalInformation[i].x,
            GEOMETRY.data.normalInformation[i].y,
            GEOMETRY.data.normalInformation[i].z);
        frame->setNormal(i, tempVec);
        tempPoint.set(
            GEOMETRY.data.textureMappingInformation[i].u,
            GEOMETRY.data.textureMappingInformation[i].v);
        if (t_invertT)
            tempPoint.y = 1.0F - tempPoint.y;
        frame->setST(i, tempPoint);
    }

#define CURR_SPLIT GEOMETRY.extension.materialSplit.splitInformation[i]
    u32 materialsCount = GEOMETRY.extension.materialSplit.header.splitCount;
    frame->allocateMaterials(materialsCount);
    for (u32 i = 0; i < materialsCount; i++)
    {
        u32 facesCount = CURR_SPLIT.faceIndex;
        u32 materialIndex = CURR_SPLIT.materialIndex;
        frame->getMaterial(i).allocateFaces(facesCount);
        char **materialName = &GEOMETRY.materialList.materials[materialIndex].textures[0].textureName.text;
        frame->getMaterial(i).setName(*materialName);
        for (u32 j = 0; j < facesCount; j++)
        {
            frame->getMaterial(i).setVertexFace(j, CURR_SPLIT.vertexInformation[j].vertex1);
            frame->getMaterial(i).setNormalFace(j, CURR_SPLIT.vertexInformation[j].vertex1);
            frame->getMaterial(i).setSTFace(j, CURR_SPLIT.vertexInformation[j].vertex1);
        }
    }
}

void DffLoader::serialize(RwClump &t_clump, u8 *t_data, float t_scale)
{
    u32 ptrPos = 0;
    readSectionHeader(t_clump, t_data, ptrPos);
    readSectionHeader(t_clump.data, t_data, ptrPos);
    readClumpData(t_clump.data, t_data, ptrPos);
    readSectionHeader(t_clump.frameList, t_data, ptrPos);
    readSectionHeader(t_clump.frameList.data, t_data, ptrPos);
    readFrameListData(t_clump.frameList.data, t_data, ptrPos);

    t_clump.frameList.extensions = new RwFrameListExtension[t_clump.frameList.data.frameCount];
    for (u32 i = 0; i < t_clump.frameList.data.frameCount; i++)
    {
        readSectionHeader(t_clump.frameList.extensions[i], t_data, ptrPos);
        ptrPos += t_clump.frameList.extensions[i].sectionSize;
    }

    readSectionHeader(t_clump.geometryList, t_data, ptrPos);
    readSectionHeader(t_clump.geometryList.data, t_data, ptrPos);
    readGeometryListData(t_clump.geometryList.data, t_data, ptrPos);

    t_clump.geometryList.geometries = new RwGeometry[t_clump.geometryList.data.geometryCount];
    printf("Geometries:%d\n", t_clump.geometryList.data.geometryCount);
    for (u32 i = 0; i < t_clump.geometryList.data.geometryCount; i++)
    {
        readSectionHeader(t_clump.geometryList.geometries[i], t_data, ptrPos);
        readSectionHeader(t_clump.geometryList.geometries[i].data, t_data, ptrPos);
        readGeometryData(t_clump.geometryList.geometries[i].data, t_data, ptrPos, t_scale);

        readSectionHeader(t_clump.geometryList.geometries[i].materialList, t_data, ptrPos);
        readSectionHeader(t_clump.geometryList.geometries[i].materialList.data, t_data, ptrPos);
        readMaterialListData(t_clump.geometryList.geometries[i].materialList.data, t_data, ptrPos);

        u32 materialCount = t_clump.geometryList.geometries[i].materialList.data.materialCount;
        printf("Materials:%d\n", materialCount);

        t_clump.geometryList.geometries[i].materialList.materials = new RwMaterial[materialCount];
        for (u32 j = 0; j < materialCount; j++)
        {
            readSectionHeader(t_clump.geometryList.geometries[i].materialList.materials[j], t_data, ptrPos);
            readSectionHeader(t_clump.geometryList.geometries[i].materialList.materials[j].data, t_data, ptrPos);
            readMaterialData(t_clump.geometryList.geometries[i].materialList.materials[j].data, t_data, ptrPos);

            u32 textureCount = t_clump.geometryList.geometries[i].materialList.materials[j].data.textureCount;
            t_clump.geometryList.geometries[i].materialList.materials[j].textures = new RwTexture[textureCount];
            for (u32 k = 0; k < textureCount; k++)
            {
                readSectionHeader(t_clump.geometryList.geometries[i].materialList.materials[j].textures[k], t_data, ptrPos);
                readSectionHeader(t_clump.geometryList.geometries[i].materialList.materials[j].textures[k].data, t_data, ptrPos);
                readTextureData(t_clump.geometryList.geometries[i].materialList.materials[j].textures[k].data, t_data, ptrPos);
                readSectionHeader(t_clump.geometryList.geometries[i].materialList.materials[j].textures[k].textureName, t_data, ptrPos);
                readStringData(t_clump.geometryList.geometries[i].materialList.materials[j].textures[k].textureName, t_data, ptrPos);
                readSectionHeader(t_clump.geometryList.geometries[i].materialList.materials[j].textures[k].textureAlphaName, t_data, ptrPos);
                readStringData(t_clump.geometryList.geometries[i].materialList.materials[j].textures[k].textureAlphaName, t_data, ptrPos);

                // The content of extension is Sky Mipmap Val
                readSectionHeader(t_clump.geometryList.geometries[i].materialList.materials[j].textures[k].extension, t_data, ptrPos);
                ptrPos += t_clump.geometryList.geometries[i].materialList.materials[j].textures[k].extension.sectionSize;
            }
            // Probably always void extension
            readSectionHeader(t_clump.geometryList.geometries[i].materialList.materials[j].extension, t_data, ptrPos);
            ptrPos += t_clump.geometryList.geometries[i].materialList.materials[j].extension.sectionSize;
        }
        // For Bin Mesh aka Material split
        readSectionHeader(t_clump.geometryList.geometries[i].extension, t_data, ptrPos);
        readGeometryExtension(t_clump.geometryList.geometries[i].extension, t_data, ptrPos);
        ptrPos += t_clump.geometryList.geometries[i].extension.sectionSize;
    }

    readSectionHeader(t_clump.atomic, t_data, ptrPos);
    readSectionHeader(t_clump.atomic.data, t_data, ptrPos);
    ptrPos += t_clump.atomic.data.sectionSize;
}

void DffLoader::readSectionHeader(RwSectionHeader &t_sh, u8 *t_buffer, u32 &t_ptrPos)
{
    t_sh.sectionType = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_sh.sectionSize = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_sh.versionNumber = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
}

// ---

void DffLoader::readFrameListData(RwFrameListData &t_frd, u8 *t_buffer, u32 &t_ptrPos)
{
    t_frd.frameCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    printf("Frame count:%d\n", t_frd.frameCount);
    t_frd.frameInformation = new RwFrameListChunk[t_frd.frameCount];

    for (u32 i = 0; i < t_frd.frameCount; i++)
    {
        t_frd.frameInformation[i].rotationalMatrix = new float[t_frd.ROT_MAT_DIM];
        for (u32 j = 0; j < t_frd.ROT_MAT_DIM; j++)
            t_frd.frameInformation[i].rotationalMatrix[j] = readFloatFromArrayLE(t_buffer, t_ptrPos);

        t_frd.frameInformation[i].coordinatesOffsetX = readFloatFromArrayLE(t_buffer, t_ptrPos);
        t_frd.frameInformation[i].coordinatesOffsetY = readFloatFromArrayLE(t_buffer, t_ptrPos);
        t_frd.frameInformation[i].coordinatesOffsetZ = readFloatFromArrayLE(t_buffer, t_ptrPos);
        t_frd.frameInformation[i]
            .parentFrame = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
        t_frd.frameInformation[i].unk1 = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    }
}

void DffLoader::readClumpData(RwClumpData &t_cd, u8 *t_buffer, u32 &t_ptrPos)
{
    t_cd.objectCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_cd.unk1 = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_cd.unk2 = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
}

void DffLoader::readGeometryListData(RwGeometryListData &t_gld, u8 *t_buffer, u32 &t_ptrPos)
{
    t_gld.geometryCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
}

void DffLoader::readGeometryExtension(RwGeometryExtension &t_ge, u8 *t_buffer, u32 &t_ptrPos)
{
    for (u8 i = 0; i < 3; i++)
        t_ptrPos += sizeof(u32);

    t_ge.materialSplit.header.triangleStrip = readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_ge.materialSplit.header.splitCount = readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_ge.materialSplit.header.faceCount = readDwordFromArrayLE(t_buffer, t_ptrPos);

    t_ge.materialSplit.splitInformation = new RwGeometryListInfoChunk[t_ge.materialSplit.header.splitCount];
    for (u32 i = 0; i < t_ge.materialSplit.header.splitCount; i++)
    {
        t_ge.materialSplit.splitInformation[i].faceIndex = readDwordFromArrayLE(t_buffer, t_ptrPos);
        t_ge.materialSplit.splitInformation[i].materialIndex = readDwordFromArrayLE(t_buffer, t_ptrPos);

        t_ge.materialSplit.splitInformation[i].vertexInformation = new RwGeometryListVertexInfoChunk[t_ge.materialSplit.splitInformation[i].faceIndex];
        for (u32 j = 0; j < t_ge.materialSplit.splitInformation[i].faceIndex; j++)
            t_ge.materialSplit.splitInformation[i].vertexInformation[j].vertex1 = readDwordFromArrayLE(t_buffer, t_ptrPos);
    }
}

void DffLoader::readGeometryData(RwGeometryData &t_gd, u8 *t_buffer, u32 &t_ptrPos, float t_scale)
{
    t_gd.dataHeader.flags = readWordFromArrayLE(t_buffer, t_ptrPos);
    t_gd.dataHeader.unk1 = readWordFromArrayLE(t_buffer, t_ptrPos);
    t_gd.dataHeader.triangleCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_gd.dataHeader.vertexCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_gd.dataHeader.morphTargetCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);

    if (false)
    { // version equals 4099 according to old documentation, disabled as for now
        t_gd.lightingHeader.ambient = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
        t_gd.lightingHeader.diffuse = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
        t_gd.lightingHeader.specular = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    }

    if (t_gd.dataHeader.flags & rwOBJECT_VERTEX_PRELIT)
    {
        t_gd.colorInformation = new RwGeometryDataColorInformationChunk[t_gd.dataHeader.vertexCount];
        for (u32 i = 0; i < t_gd.dataHeader.vertexCount; i++)
        {
            t_gd.colorInformation[i].red = readByteFromArrayLE(t_buffer, t_ptrPos);
            t_gd.colorInformation[i].green = readByteFromArrayLE(t_buffer, t_ptrPos);
            t_gd.colorInformation[i].blue = readByteFromArrayLE(t_buffer, t_ptrPos);
            t_gd.colorInformation[i].alpha = readByteFromArrayLE(t_buffer, t_ptrPos);
        }
    }

    if (t_gd.dataHeader.flags & rwOBJECT_VERTEX_TEXTURED)
    {
        t_gd.textureMappingInformation = new RwGeometryDataTextureMappingInformationChunk[t_gd.dataHeader.vertexCount];
        for (u32 i = 0; i < t_gd.dataHeader.vertexCount; i++)
        {
            t_gd.textureMappingInformation[i].u = readFloatFromArrayLE(t_buffer, t_ptrPos);
            t_gd.textureMappingInformation[i].v = readFloatFromArrayLE(t_buffer, t_ptrPos);
        }
    }

    t_gd.faceInformation = new RwGeometryDataFaceInformation[t_gd.dataHeader.triangleCount];
    for (u32 i = 0; i < t_gd.dataHeader.triangleCount; i++)
    {
        t_gd.faceInformation[i].vertex2 = readWordFromArrayLE(t_buffer, t_ptrPos);
        t_gd.faceInformation[i].vertex1 = readWordFromArrayLE(t_buffer, t_ptrPos);
        t_gd.faceInformation[i].flags = readWordFromArrayLE(t_buffer, t_ptrPos);
        t_gd.faceInformation[i].vertex3 = readWordFromArrayLE(t_buffer, t_ptrPos);
    }

    t_gd.nonameInfo.boundingSphereX = readFloatFromArrayLE(t_buffer, t_ptrPos);
    t_gd.nonameInfo.boundingSphereY = readFloatFromArrayLE(t_buffer, t_ptrPos);
    t_gd.nonameInfo.boundingSphereZ = readFloatFromArrayLE(t_buffer, t_ptrPos);
    t_gd.nonameInfo.boundingSphereR = readFloatFromArrayLE(t_buffer, t_ptrPos);
    t_gd.nonameInfo.hasPosition = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_gd.nonameInfo.hasNormals = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);

    t_gd.vertexInformation = new Vector3[t_gd.dataHeader.vertexCount];
    for (u32 i = 0; i < t_gd.dataHeader.vertexCount; i++)
    {
        t_gd.vertexInformation[i].x = readFloatFromArrayLE(t_buffer, t_ptrPos) * t_scale;
        t_gd.vertexInformation[i].y = readFloatFromArrayLE(t_buffer, t_ptrPos) * t_scale;
        t_gd.vertexInformation[i].z = readFloatFromArrayLE(t_buffer, t_ptrPos) * t_scale;
    }

    if (t_gd.dataHeader.flags & rwOBJECT_VERTEX_NORMALS)
    {
        t_gd.normalInformation = new Vector3[t_gd.dataHeader.vertexCount];
        for (u32 i = 0; i < t_gd.dataHeader.vertexCount; i++)
        {
            t_gd.normalInformation[i].x = readFloatFromArrayLE(t_buffer, t_ptrPos);
            t_gd.normalInformation[i].y = readFloatFromArrayLE(t_buffer, t_ptrPos);
            t_gd.normalInformation[i].z = readFloatFromArrayLE(t_buffer, t_ptrPos);
        }
    }
}

void DffLoader::readMaterialListData(RwMaterialListData &t_mld, u8 *t_buffer, u32 &t_ptrPos)
{
    t_mld.materialCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_mld.arrayOfUnks = new u32[t_mld.materialCount];
    for (u32 i = 0; i < t_mld.materialCount; i++)
    {
        t_mld.arrayOfUnks[i] = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    }
}

void DffLoader::readMaterialData(RwMaterialData &t_md, u8 *t_buffer, u32 &t_ptrPos)
{
    t_md.unk1 = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_md.R = readByteFromArrayLE(t_buffer, t_ptrPos);
    t_md.G = readByteFromArrayLE(t_buffer, t_ptrPos);
    t_md.B = readByteFromArrayLE(t_buffer, t_ptrPos);
    t_md.A = readByteFromArrayLE(t_buffer, t_ptrPos);
    t_md.unk2 = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_md.textureCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    t_md.unkPosX = readFloatFromArrayLE(t_buffer, t_ptrPos);
    t_md.unkPosY = readFloatFromArrayLE(t_buffer, t_ptrPos);
    t_md.unkPosZ = readFloatFromArrayLE(t_buffer, t_ptrPos);
}

void DffLoader::readTextureData(RwTextureData &t_td, u8 *t_buffer, u32 &t_ptrPos)
{
    t_td.textureFilterModeFlags = readWordFromArrayLE(t_buffer, t_ptrPos);
    t_td.unk = readWordFromArrayLE(t_buffer, t_ptrPos);
}

void DffLoader::readStringData(RwString &t_s, u8 *t_buffer, u32 &t_ptrPos)
{
    t_s.text = readString(t_buffer, t_ptrPos, t_s.sectionSize);
}

// ---

u8 DffLoader::readByteFromArrayLE(u8 *t_buffer, u32 &t_ptrPos)
{
    return t_buffer[t_ptrPos++];
}

u16 DffLoader::readWordFromArrayLE(u8 *t_buffer, u32 &t_ptrPos)
{
    u16 res = (t_buffer[t_ptrPos + 1] << 8) + (t_buffer[t_ptrPos]);
    t_ptrPos += sizeof(u16);
    return res;
}

u32 DffLoader::readDwordFromArrayLE(u8 *t_buffer, u32 &t_ptrPos)
{
    u32 res = (t_buffer[t_ptrPos + 3] << 24) + (t_buffer[t_ptrPos + 2] << 16) + (t_buffer[t_ptrPos + 1] << 8) + (t_buffer[t_ptrPos]);
    t_ptrPos += sizeof(u32);
    return res;
}

float DffLoader::readFloatFromArrayLE(u8 *t_buffer, u32 &t_ptrPos)
{
    float res;
    memcpy(&res, t_buffer + t_ptrPos, sizeof(float));
    t_ptrPos += sizeof(float);
    return res;
}

u8 *DffLoader::readData(u8 *t_buffer, u32 &t_ptrPos, u32 t_dataSize)
{
    u8 *arr = new u8[t_dataSize];
    for (u32 i = 0; i < t_dataSize; ++i)
        arr[i] = t_buffer[i + t_ptrPos];
    t_ptrPos += t_dataSize;
    return arr;
}

char *DffLoader::readString(u8 *t_buffer, u32 &t_ptrPos, u32 t_dataSize)
{
    u32 strLength = String::getLength((char *)&t_buffer[t_ptrPos]);
    char *result = new char[strLength + 1];
    for (u32 i = 0; i < strLength; i++)
        result[i] = (char)t_buffer[t_ptrPos + i];
    result[strLength] = '\0';
    t_ptrPos += t_dataSize;
    return result;
}
