/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com> 
*/

#ifndef _TYRA_TXD_LOADER_
#define _TYRA_TXD_LOADER_
#include "txd_structure.hpp"

//The Texture dictionary loader(TXD)
class TxdLoader
{
 public:
    TxdLoader();
    ~TxdLoader();
   void loadTextures(char *t_filename, RwRasterFormat *t_td, u8 *t_buffer, u32 &t_ptrPos);
private:
    u8 readByteFromArrayLE(u8 *t_buffer, u32 &t_ptrPos);
    u16 readWordFromArrayLE(u8 *t_buffer, u32 &t_ptrPos);
    u32 readDwordFromArrayLE(u8 *t_buffer, u32 &t_ptrPos);
    float readFloatFromArrayLE(u8 *t_buffer, u32 &t_ptrPos);
    void readSectionHeader(RwSectionHeader &t_sh, u8 *t_buffer, u32 &t_ptrPos);
    float readString(u8 *t_buffer, u32 *t_ptrPos, u32 t_dataSize);    
    void ReadSectionHeader(RwSectionHeader &t_sh, u8 *t_buffer, u32 &t_ptrPos);
    void ReadHeader(u32 ptrPos, u8 *t_data);
    void Read_Default(RwRasterFormat *DEFAULT_FORMAT, u8 *t_buffer, u32 &t_ptrPos);
};
#endif
