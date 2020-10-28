/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_TEXTURE_
#define _TYRA_TEXTURE_

#include <tamtypes.h>
#include <draw_sampling.h>

struct Texture
{
    u32 id;
    unsigned char *data;
    texwrap_t wrapSettings;
    char *name;
    u8 width, height;
};

#endif
