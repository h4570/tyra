
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/renderer_3d_utility.hpp"

namespace Tyra {

Renderer3DUtility::Renderer3DUtility() {}

Renderer3DUtility::~Renderer3DUtility() {}

void Renderer3DUtility::drawPoint(const Vec4& v) { drawPoint(v, 1.0F); }
void Renderer3DUtility::drawLine(const Vec4& from, const Vec4& to) {
  drawLine(from, to, 1.0F);
}
void Renderer3DUtility::drawBBox(const CoreBBox& v) { drawBBox(v, 1.0F); }

void Renderer3DUtility::drawPoint(const Vec4& v, const float& size) {
  drawPoint(v, size, Color(128.0F, 0.0F, 0.0F, 128.0F));
}

void Renderer3DUtility::drawLine(const Vec4& from, const Vec4& to,
                                 const float& size) {
  drawLine(from, to, size, Color(128.0F, 0.0F, 0.0F, 128.0F));
}

void Renderer3DUtility::drawBBox(const CoreBBox& v, const float& size) {
  drawBBox(v, size, Color(128.0F, 0.0F, 0.0F, 128.0F));
}

void Renderer3DUtility::drawPoint(const Vec4& v, const float& size,
                                  const Color& color) {
  // TODO
}

void Renderer3DUtility::drawLine(const Vec4& from, const Vec4& to,
                                 const float& size, const Color& color) {
  // TODO
}

void Renderer3DUtility::drawBBox(const CoreBBox& v, const float& size,
                                 const Color& color) {
  // TODO
}

}  // namespace Tyra
