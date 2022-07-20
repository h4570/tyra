/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
*/

#include "../include/utils/Tyra_version.hpp"
#include "../include/utils/debug.hpp"

Tyra_Version::Tyra_Version()
{

}

Tyra_Version::~Tyra_Version()
{

}

void Tyra_Version::GetVersion(Tyra_Version &version)
{
  Tyra_Version::MAJOR = TYRA_MAJOR_VERSION;
  Tyra_Version::MINOR = TYRA_MINOR_VERSION;
  Tyra_Version::PATCH_LEVEL = TYRA_PATCH_LEVEL;
#ifdef ENABLE_STRING  
  Tyra_Version::VERSION = TYRA_NAME;
#else

#endif

#ifdef ENABLE_STRING
  printf("You are using tyra engine version: %u.%u.%u %s", Tyra_Version::MAJOR, Tyra_Version::MINOR, Tyra_Version::PATCH_LEVEL, Tyra_Version::VERSION); 
#else
  printf("You are using tyra engine version: %u.%u.%u", Tyra_Version::MAJOR, Tyra_Version::MINOR, Tyra_Version::PATCH_LEVEL); 
#endif
}