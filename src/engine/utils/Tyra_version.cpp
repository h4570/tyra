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

Tyra_VersionSelect::Tyra_Version()
{

}

Tyra_VersionSelect::~Tyra_Version()
{

}

Tyra_VersionSelect::GetVersion(Tyra_number &version)
{
  version->MAJOR = TYRA_MAJOR_VERSION;
  version->MINOR = TYRA_MINOR_VERSION;
  version->PATCH_LEVEL = TYRA_PATCH_LEVEL;
  version->VERSION = TYRA_NAME;
}