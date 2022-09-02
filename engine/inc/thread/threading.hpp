/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <time.h>

namespace Tyra {

class Threading {
 public:
  Threading();
  ~Threading();

  static void sleep(const unsigned int& ms);
  static void sleep(const timespec& tv);

  static void switchThread();

 private:
  static timespec tv;
};

}  // namespace Tyra
