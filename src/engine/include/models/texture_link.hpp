/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_TEXTURE_USAGE_
#define _TYRA_TEXTURE_USAGE_

#include <tamtypes.h>

struct TextureLink
{
    /** Mesh id or sprite id */
    u32 meshOrSpriteId;
    /** Mesh material id or 0 if is sprite */
    u32 materialId;
};

#endif
