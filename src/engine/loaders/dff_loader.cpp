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
    u8 data[fileSize];
    rewind(file);
    fread(&data, sizeof(u8), fileSize, file);
    fclose(file);
    serialize(o_result, t_invertT, data, t_scale);
    o_result->calculateBoundingBox();
    PRINT_LOG("Dff file loaded!");
}

void DffLoader::serialize(MeshFrame *o_result, u8 t_invertT, u8 *t_data, float t_scale)
{
    u32 ptrPos = 0;

    // readSectionHeader(t_clump, t_data, ptrPos);
    // readSectionHeader(t_clump.data, t_data, ptrPos);
    // readClumpData(t_clump.data, t_data, ptrPos);
    for (u8 i = 0; i < 3 * 3; i++)
        ptrPos += sizeof(u32);

    // readSectionHeader(t_clump.frameList, t_data, ptrPos);
    // readSectionHeader(t_clump.frameList.data, t_data, ptrPos);
    // readFrameListData(t_clump.frameList.data, t_data, ptrPos);
    // t_clump.frameList.extensions = new RwFrameListExtension[t_clump.frameList.data.frameCount];
    // for (u32 i = 0; i < t_clump.frameList.data.frameCount; i++)
    // {
    //     readSectionHeader(t_clump.frameList.extensions[i], t_data, ptrPos);
    //     ptrPos += t_clump.frameList.extensions[i].sectionSize;
    // }
    RwSectionHeader header1 = RwSectionHeader();
    readSectionHeader(header1, t_data, ptrPos);
    ptrPos += header1.sectionSize;

    // readSectionHeader(t_clump.geometryList, t_data, ptrPos);
    // readSectionHeader(t_clump.geometryList.data, t_data, ptrPos);
    for (u8 i = 0; i < 2 * 3; i++)
        ptrPos += sizeof(u32);

    // readGeometryListData(t_clump.geometryList.data, t_data, ptrPos);
    ptrPos += sizeof(u32);

    // readSectionHeader(geometry, t_data, ptrPos);
    // readSectionHeader(geometry.data, t_data, ptrPos);
    for (size_t i = 0; i < 2 * 3; i++)
        ptrPos += sizeof(u32);

    readGeometryData(o_result, t_invertT, t_data, ptrPos, t_scale);

    // readSectionHeader(geometry.materialList, t_data, ptrPos);
    // readSectionHeader(geometry.materialList.data, t_data, ptrPos);
    for (size_t i = 0; i < 2 * 3; i++)
        ptrPos += sizeof(u32);

    RwMaterialListData materialListData = RwMaterialListData();
    readMaterialListData(materialListData, t_data, ptrPos);
    u32 materialCount = materialListData.materialCount;

    char **materialNames = new char *[materialCount];
    for (u32 j = 0; j < materialCount; j++)
    {
        // readSectionHeader(geometry.materialList.materials[j], t_data, ptrPos);
        for (size_t i = 0; i < 3; i++)
            ptrPos += sizeof(u32);

        // RwMaterialData materialData = RwMaterialData();
        // readSectionHeader(materialData, t_data, ptrPos);
        // readMaterialData(materialData, t_data, ptrPos);
        RwSectionHeader header1 = RwSectionHeader();
        readSectionHeader(header1, t_data, ptrPos);
        ptrPos += header1.sectionSize;

        // for (u32 k = 0; k < textureCount; k++)
        // {

        // readSectionHeader(geometry.materialList.materials[j].textures[k], t_data, ptrPos);
        // readSectionHeader(geometry.materialList.materials[j].textures[k].data, t_data, ptrPos);
        for (size_t i = 0; i < 2 * 3; i++)
            ptrPos += sizeof(u32);

        // readTextureData(geometry.materialList.materials[j].textures[k].data, t_data, ptrPos);
        ptrPos += sizeof(u32);

        RwSectionHeader header1b = RwSectionHeader();
        readSectionHeader(header1b, t_data, ptrPos);
        materialNames[j] = readString(t_data, ptrPos, header1b.sectionSize);

        // readSectionHeader(geometry.materialList.materials[j].textures[k].textureAlphaName, t_data, ptrPos);
        // readStringData(geometry.materialList.materials[j].textures[k].textureAlphaName, t_data, ptrPos);
        RwSectionHeader header2 = RwSectionHeader();
        readSectionHeader(header2, t_data, ptrPos);
        ptrPos += header2.sectionSize;

        // The content of extension is Sky Mipmap Val
        // readSectionHeader(geometry.materialList.materials[j].textures[k].extension, t_data, ptrPos);
        RwSectionHeader header3 = RwSectionHeader();
        readSectionHeader(header3, t_data, ptrPos);
        ptrPos += header3.sectionSize;
        // }

        // Probably always void extension
        // readSectionHeader(geometry.materialList.materials[j].extension, t_data, ptrPos);
        RwSectionHeader header4 = RwSectionHeader();
        readSectionHeader(header4, t_data, ptrPos);
        ptrPos += header4.sectionSize;
    }
    // delete[] materialNames; // TODO

    // For Bin Mesh aka Material split
    // readSectionHeader(geometry.extension, t_data, ptrPos);
    for (size_t i = 0; i < 3; i++)
        ptrPos += sizeof(u32);
    readGeometryExtension(o_result, materialNames, t_data, ptrPos);

    for (size_t i = 0; i < materialCount; i++)
        delete[] materialNames[i];

    delete materialNames;

    // ptrPos += geometry.extension.sectionSize;

    // readSectionHeader(t_clump.atomic, t_data, ptrPos);
    // readSectionHeader(t_clump.atomic.data, t_data, ptrPos);
    // ptrPos += t_clump.atomic.data.sectionSize;
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
    // t_frd.frameCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    // printf("Frame count:%d\n", t_frd.frameCount);
    // t_frd.frameInformation = new RwFrameListChunk[t_frd.frameCount];

    // for (u32 i = 0; i < t_frd.frameCount; i++)
    // {
    //     t_frd.frameInformation[i].rotationalMatrix = new float[t_frd.ROT_MAT_DIM];
    //     for (u32 j = 0; j < t_frd.ROT_MAT_DIM; j++)
    //         t_frd.frameInformation[i].rotationalMatrix[j] = readFloatFromArrayLE(t_buffer, t_ptrPos);

    //     t_frd.frameInformation[i].coordinatesOffsetX = readFloatFromArrayLE(t_buffer, t_ptrPos);
    //     t_frd.frameInformation[i].coordinatesOffsetY = readFloatFromArrayLE(t_buffer, t_ptrPos);
    //     t_frd.frameInformation[i].coordinatesOffsetZ = readFloatFromArrayLE(t_buffer, t_ptrPos);
    //     t_frd.frameInformation[i]
    //         .parentFrame = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    //     t_frd.frameInformation[i].unk1 = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    // }
}

void DffLoader::readClumpData(RwClumpData &t_cd, u8 *t_buffer, u32 &t_ptrPos)
{
    // t_cd.objectCount = readDwordFromArrayLE(t_buffer, t_ptrPos);
    // t_cd.unk1 = readDwordFromArrayLE(t_buffer, t_ptrPos);
    // t_cd.unk2 = readDwordFromArrayLE(t_buffer, t_ptrPos);
}

void DffLoader::readGeometryListData(RwGeometryListData &t_gld, u8 *t_buffer, u32 &t_ptrPos)
{
    // t_gld.geometryCount = readDwordFromArrayLE(t_buffer, t_ptrPos);
}

void DffLoader::readGeometryExtension(MeshFrame *o_frame, char **materialNames, u8 *t_buffer, u32 &t_ptrPos)
{
    for (u8 i = 0; i < 3; i++)
        t_ptrPos += sizeof(u32);

    RwMaterialSplitHeader header = RwMaterialSplitHeader();
    header.triangleStrip = readDwordFromArrayLE(t_buffer, t_ptrPos);
    header.splitCount = readDwordFromArrayLE(t_buffer, t_ptrPos);
    header.faceCount = readDwordFromArrayLE(t_buffer, t_ptrPos);

    o_frame->allocateMaterials(header.splitCount);
    for (u32 i = 0; i < header.splitCount; i++)
    {
        u32 facesCount = readDwordFromArrayLE(t_buffer, t_ptrPos);
        u32 materialIndex = readDwordFromArrayLE(t_buffer, t_ptrPos);
        o_frame->getMaterial(i).allocateFaces(facesCount);
        o_frame->getMaterial(i).setName(materialNames[materialIndex]);
        for (u32 j = 0; j < facesCount; j++)
        {
            u32 vertex1 = readDwordFromArrayLE(t_buffer, t_ptrPos);
            o_frame->getMaterial(i).setVertexFace(j, vertex1);
            o_frame->getMaterial(i).setNormalFace(j, vertex1);
            o_frame->getMaterial(i).setSTFace(j, vertex1);
        }
    }
}

void DffLoader::readGeometryData(MeshFrame *o_frame, u8 t_invertT, u8 *t_buffer, u32 &t_ptrPos, float t_scale)
{
    Vector3 tempVec = Vector3();
    Point tempPoint = Point();

    RwGeometryDataHeader header;
    header.flags = readWordFromArrayLE(t_buffer, t_ptrPos);
    header.unk1 = readWordFromArrayLE(t_buffer, t_ptrPos);
    header.triangleCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    header.vertexCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    header.morphTargetCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);

    if (false)
    {                                             // version equals 4099 according to old documentation, disabled as for now
        readDwordFromArrayLE(t_buffer, t_ptrPos); // ambient
        readDwordFromArrayLE(t_buffer, t_ptrPos); // diffuse
        readDwordFromArrayLE(t_buffer, t_ptrPos); // specular
    }

    if (header.flags & rwOBJECT_VERTEX_PRELIT)
        for (u32 i = 0; i < header.vertexCount; i++)
        {
            readByteFromArrayLE(t_buffer, t_ptrPos); // r
            readByteFromArrayLE(t_buffer, t_ptrPos); // g
            readByteFromArrayLE(t_buffer, t_ptrPos); // b
            readByteFromArrayLE(t_buffer, t_ptrPos); // a
        }

    if (header.flags & rwOBJECT_VERTEX_TEXTURED)
    {
        o_frame->allocateSTs(header.vertexCount);
        for (u32 i = 0; i < header.vertexCount; i++)
        {
            float u = readFloatFromArrayLE(t_buffer, t_ptrPos);
            float v = readFloatFromArrayLE(t_buffer, t_ptrPos);
            tempPoint.set(u, v);
            if (t_invertT)
                tempPoint.y = 1.0F - tempPoint.y;
            o_frame->setST(i, tempPoint);
        }
    }

    for (u32 i = 0; i < header.triangleCount; i++)
    {
        readWordFromArrayLE(t_buffer, t_ptrPos); // vert2
        readWordFromArrayLE(t_buffer, t_ptrPos); // vert1
        readWordFromArrayLE(t_buffer, t_ptrPos); // flags
        readWordFromArrayLE(t_buffer, t_ptrPos); // vert3
    }

    for (size_t i = 0; i < 6; i++)
        t_ptrPos += sizeof(u32);
    // t_gd.nonameInfo.boundingSphereX = readFloatFromArrayLE(t_buffer, t_ptrPos);
    // t_gd.nonameInfo.boundingSphereY = readFloatFromArrayLE(t_buffer, t_ptrPos);
    // t_gd.nonameInfo.boundingSphereZ = readFloatFromArrayLE(t_buffer, t_ptrPos);
    // t_gd.nonameInfo.boundingSphereR = readFloatFromArrayLE(t_buffer, t_ptrPos);
    // t_gd.nonameInfo.hasPosition = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    // t_gd.nonameInfo.hasNormals = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);

    o_frame->allocateVertices(header.vertexCount);
    for (u32 i = 0; i < header.vertexCount; i++)
    {
        float x = readFloatFromArrayLE(t_buffer, t_ptrPos) * t_scale;
        float y = readFloatFromArrayLE(t_buffer, t_ptrPos) * t_scale;
        float z = readFloatFromArrayLE(t_buffer, t_ptrPos) * t_scale;
        tempVec.set(x, y, z);
        o_frame->setVertex(i, tempVec);
    }

    o_frame->allocateNormals(header.vertexCount);
    for (u32 i = 0; i < header.vertexCount; i++)
    {
        float x = readFloatFromArrayLE(t_buffer, t_ptrPos);
        float y = readFloatFromArrayLE(t_buffer, t_ptrPos);
        float z = readFloatFromArrayLE(t_buffer, t_ptrPos);
        tempVec.set(x, y, z);
        o_frame->setNormal(i, tempVec);
    }
}

void DffLoader::readMaterialListData(RwMaterialListData &t_mld, u8 *t_buffer, u32 &t_ptrPos)
{
    t_mld.materialCount = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
    // t_mld.arrayOfUnks = new u32[t_mld.materialCount];
    for (u32 i = 0; i < t_mld.materialCount; i++)
        readDwordFromArrayLE(t_buffer, t_ptrPos);
    // t_mld.arrayOfUnks[i] = DffLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
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
