/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
# This file has taken from https://github.com/mrqo/rw_parse/blob/master/rw_utils/rw_txd_structure.h 
# But adopted for Tyra engine 
*/

/* Todo: Upgrade the code */
#include "../include/loaders/txd_loader.hpp"
#include "../include/utils/debug.hpp"
#include "../include/utils/math.hpp"
#include "../include/utils/string.hpp"
#include <cstring>

RwTXDLoader::RwTXDLoader() 
{
  consoleLog("Loaded Txd File");
}

RwTXDLoader::~RwTXDLoader()
{
  consoleLog("Nothing");
}

void RwTXDLoader::loadTextures(Rw_TXD_Structure::RwRasterFormat *t_td, u8 *t_buffer, u32 *t_ptrPos, char *t_filename)
{
    consoleLog("Loading textures");
    char *path = String::createConcatenated("host:", t_filename);
    FILE *file = fopen(path, "rb");
    assertMsg(file != NULL, "Failed to load .txd file!");
    fseek(file, 0L, SEEK_END);
    long fileSize = ftell(file);
    u8 data[fileSize];
    rewind(file);
    fread(&data, sizeof(u8), fileSize, file);
    fclose(file);
    delete[] path;
    RwTXDLoader();
}

void RwTXDLoader::Read_Default(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_DEFAULT, u8 *t_buffer, u32 &t_ptrPos)
{
    RwTextureNativeData	{ readWordFromArrayLE(t_buffer, t_ptrPos) };
    readWordFromArrayLE(t_buffer, t_ptrPos);
}

float RwTXDLoader::readString(u8 *t_buffer, u32 *t_ptrPos, u32 t_dataSize)
{
    readString(t_buffer, t_ptrPos, t_dataSize);
}

u8 RwTXDLoader::readByteFromArrayLE(u8 *t_buffer, u32 &t_ptrPos)
{
    return t_buffer[t_ptrPos++];
}

u16 RwTXDLoader::readWordFromArrayLE(u8 *t_buffer, u32 &t_ptrPos)
{
    u16 res = (t_buffer[t_ptrPos + 1] << 8) + (t_buffer[t_ptrPos]);
    t_ptrPos += sizeof(u16);
    return res;
}

u32 RwTXDLoader::readDwordFromArrayLE(u8 *t_buffer, u32 &t_ptrPos)
{
    u32 res = (t_buffer[t_ptrPos + 3] << 24) + (t_buffer[t_ptrPos + 2] << 16) + (t_buffer[t_ptrPos + 1] << 8) + (t_buffer[t_ptrPos]);
    t_ptrPos += sizeof(u32);
    return res;
}

float RwTXDLoader::readFloatFromArrayLE(u8 *t_buffer, u32 &t_ptrPos)
{
    float res;
    memcpy(&res, t_buffer + t_ptrPos, sizeof(float));
    t_ptrPos += sizeof(float);
    return res;
}

