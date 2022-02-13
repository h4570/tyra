/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
*/

/* Todo: Upgrade the code */
#include "../include/loaders/txd_loader.hpp"
#include "../include/utils/debug.hpp"
#include "../include/utils/math.hpp"
#include "../include/utils/string.hpp"
#include <cstring>
#include <png.h>

/********************************
** Constructors/Deconstructors **
********************************/
TxdLoader::TxdLoader() 
{
  
}

TxdLoader::~TxdLoader()
{

}

//taken from dff loader 
void TxdLoader::ReadSectionHeader(RwSectionHeader &t_sh, u8 *t_buffer, u32 &t_ptrPos)
{
  t_sh.sectionType = TxdLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
  t_sh.sectionSize = TxdLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
  t_sh.versionNumber = TxdLoader::readDwordFromArrayLE(t_buffer, t_ptrPos);
}

void TxdLoader::Read_Default(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_DEFAULT, u8 *t_buffer, u32 &t_ptrPos)
{
  u32 ptrPos;
  u8 *t_data; 
    
  //read all headers
  for (u8 i = 0; i < 3 * 2; i++)
  {
    ptrPos += sizeof(u32);
    RwSectionHeader header1 = RwSectionHeader();
    readSectionHeader(header1, t_data, ptrPos);
    ptrPos =+ header1.sectionSize;
  }
  
  for(u8 i=0; i < 3 * 3; i++)
  {
    ptrPos += sizeof(u32);
    RwSectionHeader header2 = RwSectionHeader();
    readSectionHeader(header2, t_data, ptrPos);
    ptrPos =+ header2.sectionSize;
  }

  for (u8 i = 0; i < 3 * 4; i++)
  {
    ptrPos += sizeof(u32);
    RwSectionHeader header3 = RwSectionHeader();
    readSectionHeader(header3, t_data, ptrPos);
    ptrPos =+ header3.sectionSize;
  }

  for (u8 i = 0; i < 3 * 4; i++)
  {
    ptrPos += sizeof(u32);
    RwSectionHeader header4 = RwSectionHeader();
    readSectionHeader(header4, t_data, ptrPos);
    ptrPos =+ header4.sectionSize;
  }

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

void TxdLoader::loadTextures(Rw_TXD_Structure::RwRasterFormat *t_td, u8 *t_buffer, u32 *t_ptrPos, char *t_filename)
{
    consoleLog("Loading textures");
    char *path = String::createConcatenated("host:", t_filename);
    consoleLog("Loaded Txd File");
    FILE *file = fopen(path, "rb");
    assertMsg(file != NULL, "Failed to load .txd file!");
    fseek(file, 0L, SEEK_END);
    long fileSize = ftell(file);
    u8 data[fileSize];
    rewind(file);
    fread(&data, sizeof(u8), fileSize, file);
    fclose(file);
    TxdLoader();
}
