
/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_ANIM_STATE_
#define _TYRA_ANIM_STATE_

#include <tamtypes.h>

typedef struct
{
    u32 startFrame;
    u32 endFrame;
    float speed;
    float interpolation;
    u32 animType;
    u32 currentFrame;
    u32 nextFrame;
} AnimState;

#endif
