/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/renderer_settings.hpp"

namespace Tyra {

RendererSettings::~RendererSettings() {}

void RendererSettings::copy(RendererSettings* out, const RendererSettings* in) {
  out->width = in->width;
  out->height = in->height;
  out->near = in->near;
  out->far = in->far;
  out->projectionScale = in->projectionScale;
  out->aspectRatio = in->aspectRatio;
  out->interlacedHeightF = in->interlacedHeightF;
  out->interlacedHeightUI = in->interlacedHeightUI;
}

void RendererSettings::set(const RendererSettings& v) { copy(this, &v); }

void RendererSettings::print() const {
  auto text = getPrint();
  printf("%s\n", text.c_str());
}

std::string RendererSettings::getPrint() const {
  std::stringstream res;
  res << "RendererSettings(";
  res << "width: " << width << ", ";
  res << "height: " << height << ", ";
  res << "near: " << near << ", ";
  res << "far: " << far << ", ";
  res << "projectionScale: " << projectionScale << ", ";
  res << "aspectRatio: " << aspectRatio << ", ";
  res << "interlaced height: " << interlacedHeightF;
  res << ")";
  return res.str();
}

}  // namespace Tyra
