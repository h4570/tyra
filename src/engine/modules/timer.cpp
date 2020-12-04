/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/modules/timer.hpp"

// ----
// Constructors/Destructors
// ----

Timer::Timer() { prime(); }

Timer::~Timer() {}

// ----
// Methods
// ----

u32 Timer::getTimeDelta()
{
    time = *T3_COUNT;

    if (time < lastTime) // The counter has wrapped
        change = time + (65536 - lastTime);
    else
        change = time - lastTime;
    return change;
}

float Timer::getFPS()
{
    u32 timeDelta = this->getTimeDelta();

    if (timeDelta == 0)
        return -1.0F;

    return 15625.0F / (float)timeDelta; // PAL
}
