/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/3d/renderer_core_3d.hpp"

namespace Tyra {

RendererCore3D::RendererCore3D() { fov = 60.0F; }
RendererCore3D::~RendererCore3D() {}

void RendererCore3D::update(const CameraInfo3D& cameraInfo) {
  frustumPlanes.update(cameraInfo, fov);
  M4x4::lookAt(&view, *cameraInfo.position, *cameraInfo.looksAt);
  viewProj = projection * view;
}

void RendererCore3D::init(RendererSettings* t_settings, Path1* t_path1) {
  settings = t_settings;
  path1 = t_path1;
  frustumPlanes.init(settings, fov);
  setProjection();
  TYRA_LOG("RendererCore3D initialized!");
}

// TODO, usunac albo private core? przesunac set/get fov?
void RendererCore3D::setFov(const float& t_fov) {
  fov = t_fov;
  setProjection();
}

void RendererCore3D::setProjection() {
  projection = M4x4::perspective(
      fov, settings->getWidth(), settings->getHeight(),
      settings->getProjectionScale(), settings->getAspectRatio(),
      settings->getNear(), settings->getFar());
}

u32 RendererCore3D::uploadVU1Program(VU1Program* program, const u32& address) {
  return path1->uploadProgram(program, address);
}

void RendererCore3D::setVU1DoubleBuffers(const u16& startingAddress,
                                         const u16& bufferSize) {
  path1->setDoubleBuffer(startingAddress, bufferSize);
}

}  // namespace Tyra
