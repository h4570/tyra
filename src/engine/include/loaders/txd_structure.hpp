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

#include "png_loader.hpp"
#include <tamtypes.h>
#include "../models/texture.hpp"
#include <stdio.h>
#include <stdlib.h>

//txd structure
namespace Rw_TXD_Structure
{
  enum RwFilterMode 
  {
	rwFILTER_NONE				= 0x00,
	rwFILTER_NEAREST			= 0x01,
	rwFILTER_LINEAR				= 0x02,
	rwFILTER_MIP_NEAREST		= 0x03,
	rwFILTER_MIP_LINEAR			= 0x04,
	rwFILTER_LINEAR_MIP_NEAREST = 0x05,
	rwFILTER_LINEAR_MIP_LINEAR  = 0x06
  };
   
  enum RwAddressingMode 
  {
	rwWRAP_NONE		= 0x00,
	rwWRAP_WRAP		= 0x01,
	rwWRAP_MIRROR	= 0x02,
	rwWRAP_CLAMP	= 0x03
  };

  enum RwRasterFormat 
  {
	rwFORMAT_DEFAULT	= 0x0000,
	rwFORMAT_1555		= 0x0100,
	rwFORMAT_565		= 0x0200,
	rwFORMAT_4444		= 0x0300,
	rwFORMAT_LUM8		= 0x0400,
	rwFORMAT_8888		= 0x0500,
	rwFORMAT_888		= 0x0600,
	rwFORMAT_555		= 0x0A00,

	rwFORMAT_EXT_AUTO_MIPMAP	= 0x1000,
	rwFORMAT_EXT_PAL8			= 0x2000,
	rwFORMAT_EXT_PAL4			= 0x4000,
	rwFORMAT_EXT_MIPMAP			= 0x8000
  };
};

// SectionHeader
class RwSectionHeader 
{
 public:
	u32 sectionType;
	u32 sectionSize;
	u32 versionNumber;
};

class RwTextureNativeData : public RwSectionHeader 
{
 public:
	struct textureFormat
	{
		u32 platformId;
		u8	filterMode;
		u8	uAddressing : 4;
		u8	vAddressing : 4;
		u16 pad;
		u8 name[32];
		u8 maskName[32];
	};

	struct rasterFormat
	{
		u32 rasterFormat;

		union 
        {
			u32 d3dFormat;	// SA in (D3DFORMAT)
			u32 hasAlpha;	// III/VC
		};

		u16	width;
		u16	height;
		u8 depth;
		u8 numLevels;
		u8 rasterType;

		union 
        {
			u8 compression;	// III/VC
            
    		struct 
            {				// SA
				u8 alpha : 1;
				u8 cubeTexture : 1;
				u8 autoMipMaps : 1;
				u8 isNotRwCompatible : 1;
				u8 pad : 4;
			};
		};
	}; 
};

class RwTextureNativeExtension : public RwSectionHeader 
{
public:
};

class RwTextureNative : public RwSectionHeader 
{
public:
	RwTextureNativeData			data;
	RwTextureNativeExtension	extension;
};

class RwTextureDictionaryData : public RwSectionHeader 
{
public:

        struct RwTime
        {
            u16 New_textures_Count;
			u16 deviceId;		// 1 for D3D8, 2 for D3D9, 6 for PlayStation 2, 8 for XBOX
            u32 Old_textures_Count;
        };

};

class RwTextureDictionaryExtension : public RwSectionHeader 
{
public:
};

class RwTextureDictionary : public RwSectionHeader 
{
public:
	RwTextureDictionaryData			data;
	RwTextureNative*				textures;
	RwTextureDictionaryExtension	extension;
};
//ends here

