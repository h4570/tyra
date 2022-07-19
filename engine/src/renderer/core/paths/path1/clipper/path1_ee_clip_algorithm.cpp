/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/paths/path1/clipper/path1_ee_clip_algorithm.hpp"

namespace Tyra {

Path1EEClipAlgorithm::Path1EEClipAlgorithm() {}

Path1EEClipAlgorithm::~Path1EEClipAlgorithm() {}

float Path1EEClipAlgorithm::clipMargin = -10.0F;

void Path1EEClipAlgorithm::init(const RendererSettings& settings) {
  halfWidth = settings.getWidth() / 2;
  halfHeight = settings.getHeight() / 2;
  near = settings.getNear() - (-clipMargin);
  far = -settings.getFar();
}

void Path1EEClipAlgorithm::clip(std::vector<Path1ClipVertex>* o_vertices,
                                const std::vector<Path1ClipVertex>& vertices,
                                const Path1EEClipAlgorithmSettings& settings) {
  tempVertices.clear();

  for (u32 i = 0; i < vertices.size(); i++) {
    o_vertices->push_back(vertices.at(i));
  }

  clipAgainstPlane(*o_vertices, &tempVertices, 1, halfWidth, settings);
  clipAgainstPlane(tempVertices, o_vertices, 1, -halfWidth, settings);
  clipAgainstPlane(*o_vertices, &tempVertices, 2, halfHeight, settings);
  clipAgainstPlane(tempVertices, o_vertices, 2, -halfHeight, settings);
  clipAgainstPlane(*o_vertices, &tempVertices, 3, near, settings);
  clipAgainstPlane(tempVertices, o_vertices, 4, far, settings);
}

float Path1EEClipAlgorithm::getValueByPlane(const Path1ClipVertex& v,
                                            const int& plane) {
  switch (plane) {
    case 1:
      return v.position.x;  // x plane
    case 2:
      return v.position.y;  // y plane
    case 3:                 // z near
    case 4:
      return v.position.z;  // z far
    default:
      return 0;
  }
}

bool Path1EEClipAlgorithm::isInside(const int& plane, const float& v,
                                    const float& w,
                                    const float& planeLimitValue) {
  switch (plane) {
    case 3:
      return v <= planeLimitValue;  // near z plane
    case 4:
      return v >= planeLimitValue;  // far z plane
    default:
      return (planeLimitValue < 0) ? (v >= planeLimitValue * w)
                                   : (v <= planeLimitValue * w);
  }
}

void Path1EEClipAlgorithm::clipAgainstPlane(
    const std::vector<Path1ClipVertex>& original,
    std::vector<Path1ClipVertex>* clipped, const int& plane,
    const float& planeLimitValue,
    const Path1EEClipAlgorithmSettings& settings) {
  clipped->clear();

  for (u32 i = 0; i < original.size(); i++) {
    auto a = original.at(i);
    auto b = original.at((i + 1) % original.size());
    auto apx = getValueByPlane(a, plane);
    auto bpx = getValueByPlane(b, plane);
    auto aIsInside = isInside(plane, apx, a.position.w, planeLimitValue);
    auto bIsInside = isInside(plane, bpx, b.position.w, planeLimitValue);

    if (aIsInside) {
      clipped->push_back(a);
    }

    if (aIsInside != bIsInside) {
      auto p =
          (plane >= 3)
              ? (planeLimitValue - a.position.z) / (b.position.z - a.position.z)
              : (-a.position.w * planeLimitValue + apx) /
                    ((b.position.w - a.position.w) * planeLimitValue -
                     (bpx - apx));

      Path1ClipVertex nb = {
          Vec4::getByLerp(a.position, b.position, p),
          settings.lerpNormals ? Vec4::getByLerp(a.normal, b.normal, p)
                               : Vec4(),
          settings.lerpTexCoords ? Vec4::getByLerp(a.st, b.st, p) : Vec4(),
          settings.lerpColors ? Vec4::getByLerp(a.color, b.color, p) : Vec4(),
      };

      clipped->push_back(nb);
    }
  }
}

}  // namespace Tyra
