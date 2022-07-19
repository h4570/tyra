/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020-2022, tyra - https://github.com/h4570/tyrav2
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include "thread/threading.hpp"
#include <time.h>

namespace Tyra {

void Threading::sleep(const u32& ms) {
  struct timespec tv = {0};
  tv.tv_sec = ms / 1000;
  tv.tv_nsec = (ms % 1000) * 1000000;
  nanosleep(&tv, nullptr);
}

void Threading::switchThread() { sleep(1); }

}  // Namespace Tyra