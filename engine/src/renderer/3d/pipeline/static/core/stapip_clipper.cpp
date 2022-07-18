/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/static/core/stapip_clipper.hpp"

namespace Tyra {

StaPipClipper::StaPipClipper() {}
StaPipClipper::~StaPipClipper() {}

void StaPipClipper::setMVP(M4x4* t_mvp) { mvp = t_mvp; }

void StaPipClipper::init(const RendererSettings& settings) {
  algorithm.init(settings);
}

void StaPipClipper::setMaxVertCount(const u32& count) { maxVertCount = count; }

void StaPipClipper::clip(StaPipQBuffer* buffer) {
  TYRA_ASSERT(buffer->size <= maxVertCount / 3, "Buffer should have max ",
              maxVertCount / 3, " verts if we want to clip it.");

  Path1EEClipAlgorithmSettings algoSettings = {
      buffer->bag->lighting != nullptr, buffer->bag->texture != nullptr,
      buffer->bag->color->many != nullptr};

  std::vector<Path1ClipVertex> clippedVertices;

  for (u32 i = 0; i < buffer->size / 3; i++) {
    std::vector<Path1ClipVertex> inputTriangle;
    for (u8 j = 0; j < 3; j++) {
      Path1ClipVertex vert = {
          *mvp * buffer->vertices[i * 3 + j],
          buffer->bag->lighting ? buffer->normals[i * 3 + j] : Vec4(),
          buffer->bag->texture ? buffer->sts[i * 3 + j] : Vec4(),
          buffer->bag->color->many ? buffer->colors[i * 3 + j] : Vec4()};

      inputTriangle.push_back(vert);
    }

    std::vector<Path1ClipVertex> clippedTriangle;
    algorithm.clip(&clippedTriangle, inputTriangle, algoSettings);

    if (clippedTriangle.size() == 0) continue;

    auto va = clippedTriangle.at(0);
    for (u32 j = 1; j <= clippedTriangle.size() - 2; j++) {
      auto vb = clippedTriangle.at(j);
      auto vc = clippedTriangle.at((j + 1) % clippedTriangle.size());
      clippedVertices.push_back(va);
      clippedVertices.push_back(vb);
      clippedVertices.push_back(vc);
    }
  }

  perspectiveDivide(&clippedVertices);
  moveDataToBuffer(clippedVertices, buffer);
}

void StaPipClipper::perspectiveDivide(std::vector<Path1ClipVertex>* vertices) {
  for (u32 i = 0; i < vertices->size(); i++) {
    (*vertices)[i].position /= (*vertices)[i].position.w;
  }
}

void StaPipClipper::moveDataToBuffer(
    const std::vector<Path1ClipVertex>& vertices, StaPipQBuffer* buffer) {
  buffer->reallocateManually(vertices.size());

  for (u32 i = 0; i < vertices.size(); i++) {
    auto& vertex = vertices.at(i);
    buffer->vertices[i] = vertex.position;

    if (buffer->bag->texture) buffer->sts[i] = vertex.st;
    if (buffer->bag->color->many) buffer->colors[i] = vertex.color;
    if (buffer->bag->lighting) buffer->normals[i] = vertex.normal;
  }
}

}  // namespace Tyra
