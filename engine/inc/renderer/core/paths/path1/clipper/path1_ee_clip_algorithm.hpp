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
#include "./path1_clip_vertex.hpp"
#include "debug/debug.hpp"
#include "renderer/renderer_settings.hpp"

namespace Tyra {

struct Path1EEClipAlgorithmSettings {
  bool lerpNormals, lerpTexCoords, lerpColors;
};

class Path1EEClipAlgorithm {
 public:
  Path1EEClipAlgorithm();
  ~Path1EEClipAlgorithm();

  void init(const RendererSettings& settings);

  void clip(std::vector<Path1ClipVertex>* o_vertices,
            const std::vector<Path1ClipVertex>& vertices,
            const Path1EEClipAlgorithmSettings& settings);

  static float clipMargin;

 private:
  float halfWidth, halfHeight, near, far;
  std::vector<Path1ClipVertex> tempVertices;

  float getValueByPlane(const Path1ClipVertex& v, const int& plane);

  bool isInside(const int& plane, const float& v, const float& w,
                const float& planeLimitValue);

  void clipAgainstPlane(const std::vector<Path1ClipVertex>& original,
                        std::vector<Path1ClipVertex>* clipped, const int& plane,
                        const float& planeLimitValue,
                        const Path1EEClipAlgorithmSettings& settings);
};

}  // namespace Tyra
