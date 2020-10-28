

/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_SCREEN_SETTINGS_
#define _TYRA_SCREEN_SETTINGS_

struct ScreenSettings
{
    float fov;
    float width;
    float height;
    float aspectRatio;
    float nearPlaneDist;
    float farPlaneDist;
    /** change it to 4096.0F to check out frustum culling! 😎 */
    float projectionScale;
};

#endif
