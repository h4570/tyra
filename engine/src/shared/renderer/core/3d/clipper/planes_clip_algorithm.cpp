/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/renderer/core/3d/clipper/planes_clip_algorithm.hpp"

namespace Tyra {

PlanesClipAlgorithm::PlanesClipAlgorithm() {
  tempVertices = new PlanesClipVertex[9];
}

PlanesClipAlgorithm::~PlanesClipAlgorithm() { delete[] tempVertices; }

float PlanesClipAlgorithm::clipMargin = -10.0F;

void PlanesClipAlgorithm::init(const RendererSettings& settings) {
  halfWidth = 0.5F;
  halfHeight = 0.5F;
  near = settings.getNear() - (-clipMargin);
  far = -settings.getFar();
}

unsigned char PlanesClipAlgorithm::clip(
    PlanesClipVertex* o_vertices, PlanesClipVertexPtrs* i_vertices,
    const EEClipAlgorithmSettings& settings) {
  for (int i = 0; i < 3; i++) {
    o_vertices[i].position = *i_vertices[i].position;
    if (settings.lerpColors) o_vertices[i].color = *i_vertices[i].color;
    if (settings.lerpNormals) o_vertices[i].normal = *i_vertices[i].normal;
    if (settings.lerpTexCoords) o_vertices[i].st = *i_vertices[i].st;
  }

  unsigned char tempVerticesSize = 0;
  unsigned char outputSize = 0;

  tempVerticesSize =
      clipAgainstPlane(o_vertices, 3, tempVertices, 1, halfWidth, settings);

  outputSize = clipAgainstPlane(tempVertices, tempVerticesSize, o_vertices, 1,
                                -halfWidth, settings);

  tempVerticesSize = clipAgainstPlane(o_vertices, outputSize, tempVertices, 2,
                                      halfHeight, settings);

  outputSize = clipAgainstPlane(tempVertices, tempVerticesSize, o_vertices, 2,
                                -halfHeight, settings);

  tempVerticesSize =
      clipAgainstPlane(o_vertices, outputSize, tempVertices, 3, near, settings);

  outputSize = clipAgainstPlane(tempVertices, tempVerticesSize, o_vertices, 4,
                                far, settings);

  return outputSize;
}

float PlanesClipAlgorithm::getValueByPlane(const PlanesClipVertex& v,
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

bool PlanesClipAlgorithm::isInside(const int& plane, const float& v,
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

unsigned char PlanesClipAlgorithm::clipAgainstPlane(
    PlanesClipVertex* original, const unsigned char& originalSize,
    PlanesClipVertex* clipped, const int& plane, const float& planeLimitValue,
    const EEClipAlgorithmSettings& settings) {
  int clippedSize = 0;

  for (unsigned int i = 0; i < originalSize; i++) {
    auto a = original[i];
    auto b = original[(i + 1) % originalSize];
    auto apx = getValueByPlane(a, plane);
    auto bpx = getValueByPlane(b, plane);
    auto aIsInside = isInside(plane, apx, a.position.w, planeLimitValue);
    auto bIsInside = isInside(plane, bpx, b.position.w, planeLimitValue);

    if (aIsInside) {
      clipped[clippedSize++] = a;
    }

    if (aIsInside != bIsInside) {
      auto p =
          (plane >= 3)
              ? (planeLimitValue - a.position.z) / (b.position.z - a.position.z)
              : (-a.position.w * planeLimitValue + apx) /
                    ((b.position.w - a.position.w) * planeLimitValue -
                     (bpx - apx));

      auto index = clippedSize++;

      clipped[index].position = Vec4::getByLerp(a.position, b.position, p);

      if (settings.lerpNormals)
        clipped[index].normal = Vec4::getByLerp(a.normal, b.normal, p);

      if (settings.lerpTexCoords)
        clipped[index].st = Vec4::getByLerp(a.st, b.st, p);

      if (settings.lerpColors)
        clipped[index].color = Vec4::getByLerp(a.color, b.color, p);
    }
  }

  return clippedSize;
}

}  // namespace Tyra
