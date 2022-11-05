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

namespace Tyra {

enum PipelineTransformationType {
  /** Multiplies model by view and projection matrix */
  TyraMVP,
  /** Multiplies model by only projection matrix */
  TyraMP,
};

}  // namespace Tyra
