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

  void clip(std::vector<EEClipVertex>* o_vertices,
            const std::vector<EEClipVertex>& vertices,
            const EEClipAlgorithmSettings& settings);

  static float clipMargin;

 private:
  float halfWidth, halfHeight, near, far;
  std::vector<EEClipVertex> tempVertices;

  float getValueByPlane(const EEClipVertex& v, const int& plane);

  bool isInside(const int& plane, const float& v, const float& w,
                const float& planeLimitValue);

  void clipAgainstPlane(const std::vector<EEClipVertex>& original,
                        std::vector<EEClipVertex>* clipped, const int& plane,
                        const float& planeLimitValue,
                        const EEClipAlgorithmSettings& settings);
};

}  // namespace Tyra
