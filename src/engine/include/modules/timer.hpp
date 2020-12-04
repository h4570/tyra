/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_TIMER_
#define _TYRA_TIMER_

#include <tamtypes.h>
#include <timer.h>

/** Class responsible for fps counting */
class Timer
{

public:
    Timer();
    ~Timer();

    u32 getTimeDelta();
    inline void prime() { lastTime = *T3_COUNT; }
    float getFPS();

private:
    u32 lastTime, time, change;
};

#endif
