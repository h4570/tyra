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

#include "renderer/models/color.hpp"

namespace Tyra {

/**
 * @brief Color data. At least one color data is required (single/many).
 */
class StaPipColorBag {
 public:
  StaPipColorBag();
  ~StaPipColorBag();

  /** Optional. Single color for all vertices. */
  const Color* single;

  /** Optional. Color per vertex. */
  const Color* many;
};

}  // namespace Tyra
