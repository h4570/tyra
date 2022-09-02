/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#pragma once

#include "shared/renderer/renderer.hpp"

namespace Tyra {

class Banner {
 public:
  Banner();
  ~Banner();

  void show(Renderer* renderer);
};

}  // namespace Tyra
