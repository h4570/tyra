/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_DFF_LOADER_
#define _TYRA_DFF_LOADER_

#include "../models/mesh_frame.hpp"
#include "./dff_structure.hpp"

/** Class responsible for loading&parsing .obj 3D files */
class DffLoader
{

public:
    DffLoader();
    ~DffLoader();

    void load(MeshFrame *o_result, char *t_fileName, float t_scale, u8 t_invertT);
    void serialize(RwClump &t_clump, u8 *t_data, float t_scale);

    static u8 readByteFromArrayLE(u8 *t_buffer, u32 &t_ptrPos);
    static u16 readWordFromArrayLE(u8 *t_buffer, u32 &t_ptrPos);
    static u32 readDwordFromArrayLE(u8 *t_buffer, u32 &t_ptrPos);
    static float readFloatFromArrayLE(u8 *t_buffer, u32 &t_ptrPos);
    static u8 *readData(u8 *t_buffer, u32 &t_ptrPos, u32 t_dataSize);
    static char *readString(u8 *t_buffer, u32 &t_ptrPos, u32 t_dataSize = 0);

    void readSectionHeader(RwSectionHeader &t_sh, u8 *t_buffer, u32 &t_ptrPos);

private:
    void convert(MeshFrame *frames, RwClump &t_clump, u8 t_invertT);
    // ---
    void readFrameListData(RwFrameListData &t_frd, u8 *t_buffer, u32 &t_ptrPos);
    void readClumpData(RwClumpData &t_cd, u8 *t_buffer, u32 &t_ptrPos);
    void readGeometryListData(RwGeometryListData &t_gld, u8 *t_buffer, u32 &t_ptrPos);
    void readGeometryExtension(RwGeometryExtension &t_ge, u8 *t_buffer, u32 &t_ptrPos);
    void readGeometryData(RwGeometryData &t_gd, u8 *t_buffer, u32 &t_ptrPos, float t_scale);
    void readMaterialListData(RwMaterialListData &t_mld, u8 *t_buffer, u32 &t_ptrPos);
    void readMaterialData(RwMaterialData &t_md, u8 *t_buffer, u32 &t_ptrPos);
    void readTextureData(RwTextureData &t_td, u8 *t_buffer, u32 &t_ptrPos);
    void readStringData(RwString &t_s, u8 *bt_uffer, u32 &t_ptrPos);
    // ---
};

#endif
