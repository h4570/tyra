/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "renderer/models/color.hpp"

namespace Tyra {

/**
 * @brief Color data. At least one color data is required (single/many).
 */
class DynPipColorBag {
 public:
  DynPipColorBag();
  ~DynPipColorBag();

  /** Optional. Single color for all vertices. */
  Color* singleFrom;
  Color* singleTo;

  /** Optional. Color per vertex. */
  Color* manyFrom;
  Color* manyTo;
};

}  // namespace Tyra
