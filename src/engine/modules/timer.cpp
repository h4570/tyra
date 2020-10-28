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

#include <timer.h>

// ----
// Constructors/Destructors
// ----

Timer::Timer()
{
    this->lastTime = *T3_COUNT;
}

Timer::~Timer() {}

// ----
// Methods
// ----

u32 Timer::getTimeDelta()
{
    this->time = *T3_COUNT;

    if (this->time < this->lastTime) // The counter has wrapped
        this->change = this->time + (65536 - this->lastTime);
    else
        this->change = this->time - this->lastTime;

    this->lastTime = this->time;
    return this->change;
}

void Timer::primeTimer()
{
    lastTime = *T3_COUNT;
}

float Timer::getFPS()
{
    u32 timeDelta = this->getTimeDelta();

    if (timeDelta == 0)
        return -1.0F;

    return 15625.0F / (float)timeDelta; // PAL
}
