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

#include <vector>
#include "./planes_clip_vertex.hpp"
#include "shared/debug/debug.hpp"
#include "shared/renderer/renderer_settings.hpp"

namespace Tyra {

struct EEClipAlgorithmSettings {
  bool lerpNormals, lerpTexCoords, lerpColors;
};

class PlanesClipAlgorithm {
 public:
  PlanesClipAlgorithm();
  ~PlanesClipAlgorithm();

  void init(const RendererSettings& settings);

  unsigned char clip(PlanesClipVertex* o_vertices,
                     PlanesClipVertexPtrs* i_vertices,
                     const EEClipAlgorithmSettings& settings);

  static float clipMargin;

 private:
  float halfWidth, halfHeight, near, far;
  PlanesClipVertex* tempVertices;

  float getValueByPlane(const PlanesClipVertex& v, const int& plane);

  bool isInside(const int& plane, const float& v, const float& w,
                const float& planeLimitValue);

  /** @return clipped size */
  unsigned char clipAgainstPlane(PlanesClipVertex* original,
                                 const unsigned char& originalSize,
                                 PlanesClipVertex* clipped, const int& plane,
                                 const float& planeLimitValue,
                                 const EEClipAlgorithmSettings& settings);
};

}  // namespace Tyra
