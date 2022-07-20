/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
*/

#ifndef TYRA_VERSION_HPP
#define TYRA_VERSION_HPP

#include <tamtypes.h>
#include <string>
#include "string.hpp"

#define TYRA_MAJOR_VERSION 1
#define TYRA_MINOR_VERSION 31 
#define TYRA_PATCH_LEVEL 3

#ifdef ENABLE_STRING
#define TYRA_NAME "Alpha"
#else
#endif

class Tyra_Version
{
public:   
     Tyra_Version();
    ~Tyra_Version();  
    void Get();
private:  
    u32 MAJOR; 
    u32 MINOR;
    u32 PATCH_LEVEL;
#ifdef ENABLE_STRING    
    std::string VERSION;  
#endif
};


#endif