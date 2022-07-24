/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
*/

#include "../include/loaders/txd_loader.hpp"
#include "../include/utils/debug.hpp"
#include "../include/utils/math.hpp"
#include "../include/utils/string.hpp"
#include <cstring>
#include <png.h>

/********************************
** Constructors/Deconstructors **
********************************/
TxdLoader::TxdLoader() {}

TxdLoader::~TxdLoader() {}

//taken from dff loader 
void TxdLoader::Read_Default(RwRasterFormat *DEFAULT_FORMAT, u8 *t_buffer, u32 &t_ptrPos)
{
   FILE *file;
   fread(&DEFAULT_FORMAT, sizeof(t_buffer), t_ptrPos, file);
}

float TxdLoader::readString(u8 *t_buffer, u32 *t_ptrPos, u32 t_dataSize)
{
    readString(t_buffer, t_ptrPos, t_dataSize);
}

u8 TxdLoader::readByteFromArrayLE(u8 *t_buffer, u32 &t_ptrPos)
{
    return t_buffer[t_ptrPos++];
}

u16 TxdLoader::readWordFromArrayLE(u8 *t_buffer, u32 &t_ptrPos)
{
    u16 res = (t_buffer[t_ptrPos + 1] << 8) + (t_buffer[t_ptrPos]);
    t_ptrPos += sizeof(u16);
    return res;
}

u32 TxdLoader::readDwordFromArrayLE(u8 *t_buffer, u32 &t_ptrPos)
{
    u32 res = (t_buffer[t_ptrPos + 3] << 24) + (t_buffer[t_ptrPos + 2] << 16) + (t_buffer[t_ptrPos + 1] << 8) + (t_buffer[t_ptrPos]);
    t_ptrPos += sizeof(u32);
    return res;
}

float TxdLoader::readFloatFromArrayLE(u8 *t_buffer, u32 &t_ptrPos)
{
    float res;
    memcpy(&res, t_buffer + t_ptrPos, sizeof(float));
    t_ptrPos += sizeof(float);
    return res;
}

void TxdLoader::ReadSectionHeader(RwSectionHeader &t_sh, u8 *t_buffer, u32 &t_ptrPos)
{
  t_sh.sectionType = TxdLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
  t_sh.sectionSize = TxdLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
  t_sh.versionNumber = TxdLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
}


void TxdLoader::ReadHeader(u32 ptrPos, u8 *t_data)
{
  u8 i;
  //read all headers
  for (i = 0; i < 3 * 2; i++)
  {
    ptrPos += sizeof(u32);
    RwSectionHeader header1;
    TxdLoader::readSectionHeader(header1, t_data, ptrPos);
    ptrPos =+ header1.sectionSize;
  }
  
  for(i=0; i < 3 * 3; i++)
  {
    ptrPos += sizeof(u32);
    RwSectionHeader header2;
    TxdLoader::readSectionHeader(header2, t_data, ptrPos);
    ptrPos =+ header2.sectionSize;
  }

  for (i = 0; i < 3 * 4; i++)
  {
    ptrPos += sizeof(u32);
    RwSectionHeader header3;
    TxdLoader::readSectionHeader(header3, t_data, ptrPos);
    ptrPos =+ header3.sectionSize;
  }

  for (i = 0; i < 3 * 5; i++)
  {
    ptrPos += sizeof(u32);
    RwSectionHeader header4;
    TxdLoader::readSectionHeader(header4, t_data, ptrPos);
    ptrPos =+ header4.sectionSize;
  }
}


void TxdLoader::loadTextures(char *t_filename, RwRasterFormat *t_td, u8 *t_buffer, u32 &t_ptrPos)
{
    //char *path;
    FILE *file;
    long fileSize;
    u8 data;
    consoleLog("Loading textures");
    memcpy(&data, t_buffer, sizeof(fileSize));
    TxdLoader::Read_Default(t_td, t_buffer, t_ptrPos);
    char *path = String::createConcatenated("host:", t_filename);
    consoleLog("Loaded Txd File");
    file = fopen(path, "rb");
    fread(&data, sizeof(u8), fileSize, file);
    fseek(file, 0L, SEEK_END);
    fileSize = ftell(file);
    assertMsg(file != NULL, "Failed to load .txd file!");
    rewind(file);
    fclose(file);
}
