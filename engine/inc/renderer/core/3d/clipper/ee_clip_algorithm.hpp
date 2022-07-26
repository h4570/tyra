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

#include <vector>
#include "./ee_clip_vertex.hpp"
#include "debug/debug.hpp"
#include "renderer/renderer_settings.hpp"

namespace Tyra {

struct EEClipAlgorithmSettings {
  bool lerpNormals, lerpTexCoords, lerpColors;
};

class EEClipAlgorithm {
 public:
  EEClipAlgorithm();
  ~EEClipAlgorithm();

  void init(const RendererSettings& settings);

  u8 clip(EEClipVertex* o_vertices, EEClipVertexPtrs* i_vertices,
          const EEClipAlgorithmSettings& settings);

  static float clipMargin;

 private:
  float halfWidth, halfHeight, near, far;
  EEClipVertex* tempVertices;

  float getValueByPlane(const EEClipVertex& v, const int& plane);

  bool isInside(const int& plane, const float& v, const float& w,
                const float& planeLimitValue);

  /** @return clipped size */
  u8 clipAgainstPlane(EEClipVertex* original, const u8& originalSize,
                      EEClipVertex* clipped, const int& plane,
                      const float& planeLimitValue,
                      const EEClipAlgorithmSettings& settings);
};

}  // namespace Tyra
