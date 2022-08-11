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

#include "renderer/3d/pipeline/shared/bag/pipeline_info_bag.hpp"
#include "renderer/3d/pipeline/static/stapip_ztest.hpp"

namespace Tyra {

class StaPipInfoBag : public PipelineInfoBag {
 public:
  StaPipInfoBag() {
    fullClipChecks = false;
    zTestType = StaPipZTest_Standard;
  }
  ~StaPipInfoBag() {}

  /**
   * @brief Experimental! False -> disables "clip against each plane" algorithm.
   * Default: True.
   *
   * Full clip checks are slow, but they are
   * preventing visual artifacts, which can happen
   * for big 3D objects (or objects near camera eyes)
   * Force enabled in dynamic pipe, because of efficiency.
   */
  bool fullClipChecks;

  /** @brief Type of z buffer testing. */
  StaPipZTest zTestType;
};

}  // namespace Tyra
