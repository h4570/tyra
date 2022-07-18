/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
*/

#include "string.hpp"
#include <tamtypes.h>

#define TYRA_MAJOR_VERSION 1
#define TYRA_MINOR_VERSION 31 
#define TYRA_PATCH_LEVEL 3
#define TYRA_NAME "Alpha"

typedef struct
{
  u32 MAJOR; 
  u32 MINOR;
  u32 PATCH_LEVEL;
  u32 STRING;
} Tyra_number;

class Tyra_VersionSelect
{
   Tyra_Version();
   ~Tyra_Version();  
   void GetVersion(Tyra_version &version)
};


#endif