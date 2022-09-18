/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyrav2
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "thread/threading.hpp"
#include <time.h>

namespace Tyra {

timespec Threading::tv = {0, 0};

void Threading::sleep(const u32& ms) {
  tv.tv_sec = ms / 1000;
  tv.tv_nsec = (ms % 1000) * 1000000;
  nanosleep(&tv, nullptr);
}

void Threading::sleep(const timespec& t_tv) { nanosleep(&t_tv, nullptr); }

void Threading::switchThread() {
  tv.tv_sec = 0;
  tv.tv_nsec = 500000;  // 1/2ms
  nanosleep(&tv, nullptr);
}

}  // Namespace Tyra
