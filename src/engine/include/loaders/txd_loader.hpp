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
   virtual void ReadSectionHeader(RwSectionHeader &t_sh, u8 *t_buffer, u32 &t_ptrPos);
private:
    void loadTextures(Rw_TXD_Structure::RwRasterFormat *t_td, u8 *t_buffer, u32 *t_ptrPos, char *t_filename);
    u8 readByteFromArrayLE(u8 *t_buffer, u32 &t_ptrPos);
    u16 readWordFromArrayLE(u8 *t_buffer, u32 &t_ptrPos);
    u32 readDwordFromArrayLE(u8 *t_buffer, u32 &t_ptrPos);
    float readFloatFromArrayLE(u8 *t_buffer, u32 &t_ptrPos);
    void readSectionHeader(RwSectionHeader &t_sh, u8 *t_buffer, u32 &t_ptrPos);
    float readString(u8 *t_buffer, u32 *t_ptrPos, u32 t_dataSize);    
    void Read_Default(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_DEFAULT, u8 *t_buffer, u32 &t_ptrPos);
};
#endif

/*
FIXME AND TODO:
    void Read_1555(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_1555, u8 *t_buffer, u32 &t_Size);
    void Read_565(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_565, u32 *t_buffer, u32 &t_Size);
    void Read_4444(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_4444, u32 t_buffer, u32 &t_Size);
    void Read_8888(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_8888, u32 t_buffer, u32 &t_Size);
    void Read_888(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_888, u32 t_buffer, u32 &t_Size);
    void Read_555(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_555, u32 t_buffer, u32 &t_Size);
    void Read_AUTO_MIPMAP(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_EXT_AUTO_MIPMAP, u32 t_buffer, u32 &t_Size);
    void Read_PAL8(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_EXT_PAL4, u32 t_buffer, u32 &t_Size);
    void Read_PAL4(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_EXT_PAL4, u32 t_buffer, u32 &t_Size);
    void Read_MIPMAP(Rw_TXD_Structure::RwRasterFormat *rwFORMAT_EXT_MIPMAP, u32 t_buffer, u32 &t_Size);
*/