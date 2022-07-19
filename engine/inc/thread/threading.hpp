/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <tamtypes.h>

namespace Tyra {

class Threading {
 public:
  Threading();
  ~Threading();

  static void sleep(const u32& ms);

  static void switchThread();
};

}  // namespace Tyra
