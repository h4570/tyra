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

namespace Tyra {

enum PipelineFrustumCulling {
  /** No frustum culling  */
  PipelineFrustumCulling_None = 0,
  /** Frustum culling of whole object */
  PipelineFrustumCulling_Simple = 1,
  /** Frustum culling of parts of an object */
  PipelineFrustumCulling_Precise = 2,
};

}
