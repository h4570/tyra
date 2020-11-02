/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_DFF_STRUCTURE_
#define _TYRA_DFF_STRUCTURE_

#include <tamtypes.h>
#include "../models/math/vector3.hpp"

enum RwGeometryDataFlags
{
    rwOBJECT_VERTEX_TRISTRIP = 0x01,
    rwOBJECT_VERTEX_POS = 0x02,
    rwOBJECT_VERTEX_TEXTURED = 0x04,
    rwOBJECT_VERTEX_PRELIT = 0x08,
    rwOBJECT_VERTEX_NORMALS = 0x10,
    rwOBJECT_VERTEX_LIGHT = 0x20,
    rwOBJECT_VERTEX_MODULATE_MATERIAL_COLOR = 0x40,
    rwOBJECT_VERTEX_TEXTURED_2 = 0x80
};

class RwSectionHeader
{
public:
    u32 sectionType;
    u32 sectionSize;
    u32 versionNumber;
};

class RwFrameListChunk
{
public:
    ~RwFrameListChunk() {}
    float *rotationalMatrix; // [ROT_MAT_DIM]
    float coordinatesOffsetX;
    float coordinatesOffsetY;
    float coordinatesOffsetZ;
    u32 parentFrame; // 0xFFFFFFFF = NONE
    u32 unk1;
};

// Section: Frame List
class RwFrameListData : public RwSectionHeader
{
public:
    ~RwFrameListData() {}
    static const u32 ROT_MAT_DIM = 9;
    u32 frameCount;

    RwFrameListChunk *frameInformation; // [frameCount]
};

class RwFrame : public RwSectionHeader
{
public:
    ~RwFrame() {}
    u8 *frameName; // [sectionSize] - should be interpreted as string
};

class RwFrameListExtension : public RwSectionHeader
{
public:
    RwFrame frame;
};

class RwFrameList : public RwSectionHeader
{
public:
    RwFrameListData data;
    ~RwFrameList() {}
    RwFrameListExtension *extensions;
};
// ---

class RwGeometryDataHeader
{
public:
    u16 flags;
    u16 unk1;
    u32 triangleCount;
    u32 vertexCount;
    u32 morphTargetCount;
};

struct RwGeometryDataLightingHeader
{ // IF versionNumber = 4099 - according to not complete documentation
    float ambient;
    float diffuse;
    float specular;
};

struct RwGeometryDataColorInformationChunk
{ // if rwOBJECT_VERTEX_PREILIT in flags

    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
};

struct RwGeometryDataTextureMappingInformationChunk
{ // if rwOBJECT_VERTEX_TEXTURED in flags
    float u;
    float v;
};

struct RwGeometryDataFaceInformation
{
    u16 vertex2; // this
    u16 vertex1; // is
    u16 flags;   // weird
    u16 vertex3; // order
};

class RwGeometryDataNonameInfo
{
public:
    float boundingSphereX;
    float boundingSphereY;
    float boundingSphereZ;
    float boundingSphereR;
    u32 hasPosition;
    u32 hasNormals;
};

#include <stdio.h>
#include <stdlib.h>

// Section: Geometry
class RwGeometryData : public RwSectionHeader
{
public:
    ~RwGeometryData()
    {
        // if (dataHeader.flags & rwOBJECT_VERTEX_NORMALS)
        // delete[] normalInformation;
        // delete[] vertexInformation;
        // free(vertexInformation);
        // delete[] faceInformation;
        // if (dataHeader.flags & rwOBJECT_VERTEX_PRELIT)
        //     delete[] colorInformation;
        // if (dataHeader.flags & rwOBJECT_VERTEX_TEXTURED)
        //     delete[] textureMappingInformation;
    }
    RwGeometryDataHeader dataHeader;

    RwGeometryDataLightingHeader lightingHeader;

    RwGeometryDataColorInformationChunk *colorInformation; // [vertexCount]

    RwGeometryDataTextureMappingInformationChunk *textureMappingInformation; // [vertexCount]

    RwGeometryDataFaceInformation *faceInformation; // [triangleCount]

    RwGeometryDataNonameInfo nonameInfo;

    Vector3 *vertexInformation; // [vertexCount]

    Vector3 *normalInformation; // [vertexCount]
};

class RwMaterialData : public RwSectionHeader
{
public:
    u32 unk1;
    u8 R;
    u8 G;
    u8 B;
    u8 A;
    u32 unk2;
    u32 textureCount;
    float unkPosX;
    float unkPosY;
    float unkPosZ;
};

class RwTextureData : public RwSectionHeader
{
public:
    u16 textureFilterModeFlags;
    u16 unk;
};

class RwString : public RwSectionHeader
{
public:
    ~RwString() { delete[] text; }
    char *text;
};

class RwTextureExtension : public RwSectionHeader
{
    // May contain Sky Mipmap Val
};

class RwTexture : public RwSectionHeader
{
public:
    RwTextureData data;
    RwString textureName;
    RwString textureAlphaName;
    RwTextureExtension extension;
};

class RwMaterialExtension : public RwSectionHeader
{
    // meant to be void.. don't ask
};

class RwMaterial : public RwSectionHeader
{
public:
    ~RwMaterial() {}
    RwMaterialData data;
    RwTexture *textures;
    RwMaterialExtension extension;
};

class RwMaterialListData : public RwSectionHeader
{
public:
    ~RwMaterialListData() {}
    u32 materialCount;
    u32 *arrayOfUnks; // Filled with '-1's, [materialCount] - it wasn't mentioned in docs.
};

class RwMaterialList : public RwSectionHeader
{
public:
    ~RwMaterialList() {}
    RwMaterialListData data;
    RwMaterial *materials;
};

class RwGeometryListVertexInfoChunk
{
public:
    u32 vertex1;
};

class RwGeometryListInfoChunk
{
public:
    ~RwGeometryListInfoChunk() {}
    u32 faceIndex;
    u32 materialIndex;

    RwGeometryListVertexInfoChunk *vertexInformation; // [faceIndex]
};

class RwMaterialSplitHeader
{
public:
    u32 triangleStrip;
    u32 splitCount;
    u32 faceCount;
};

class RwMaterialSplit
{
public:
    ~RwMaterialSplit() {}
    RwMaterialSplitHeader header;

    RwGeometryListInfoChunk *splitInformation; // [splitCount]
};

class RwGeometryExtension : public RwSectionHeader
{
public:
    RwMaterialSplit materialSplit;
};

class RwGeometry : public RwSectionHeader
{
public:
    RwGeometryData data;
    RwMaterialList materialList;
    RwGeometryExtension extension;
};
// ---

// Section: Geometry List
class RwGeometryListData : public RwSectionHeader
{
public:
    u32 geometryCount;
};

class RwGeometryList : public RwSectionHeader
{
public:
    ~RwGeometryList() {}
    RwGeometryListData data;
    RwGeometry *geometries;
};
// ---

// Section: Atomic
class RwAtomicData : public RwSectionHeader
{
public:
    u32 frameNumber;
    u32 geometryNumber;
    u32 unk1;
    u32 unk2;
};

class RwAtomic : public RwSectionHeader
{
public:
    RwAtomicData data;
};
// ---

// Section: Clump
class RwClumpData : public RwSectionHeader
{
public:
    u32 objectCount;
    u32 unk1;
    u32 unk2;
};

class RwClump : public RwSectionHeader
{
public:
    RwClumpData data;
    RwFrameList frameList;
    RwGeometryList geometryList;
    RwAtomic atomic; //multiple?? but how many
};
// ---

#endif
