/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "debug/debug.hpp"
#include <iomanip>
#include <sstream>
#include "renderer/core/3d/renderer_3d_frustum_planes.hpp"

namespace Tyra {

Renderer3DFrustumPlanes::Renderer3DFrustumPlanes() { lastFov = 0.0F; }
Renderer3DFrustumPlanes::~Renderer3DFrustumPlanes() {}

void Renderer3DFrustumPlanes::init(RendererSettings* t_settings,
                                   const float& fov) {
  settings = t_settings;
  computeStaticData(fov);

  TYRA_LOG("Frustum planes initialized!");
}

void Renderer3DFrustumPlanes::update(const CameraInfo3D& cameraInfo,
                                     const float& fov) {
  computeStaticData(fov);

  // compute the Z axis of camera
  Z = *cameraInfo.position - *cameraInfo.looksAt;
  Z.normalize();

  // X axis of camera of given "up" vector and Z axis
  X = cameraInfo.up->cross(Z);
  X.normalize();

  // the real "up" vector is the cross product of Z and X
  Y = Z.cross(X);

  // compute the center of the near and far planes
  nearCenter = *cameraInfo.position - Z * settings->getNear();
  farCenter = *cameraInfo.position - Z * settings->getFar();

  // compute the 8 corners of the frustum
  ntl = nearCenter + Y * nearHeight - X * nearWidth;
  ntr = nearCenter + Y * nearHeight + X * nearWidth;
  nbl = nearCenter - Y * nearHeight - X * nearWidth;
  nbr = nearCenter - Y * nearHeight + X * nearWidth;

  ftl = farCenter + Y * farHeight - X * farWidth;
  fbr = farCenter - Y * farHeight + X * farWidth;
  ftr = farCenter + Y * farHeight + X * farWidth;
  fbl = farCenter - Y * farHeight - X * farWidth;

  frustumPlanes[0].update(ntr, ntl, ftl);  // Top
  frustumPlanes[1].update(nbl, nbr, fbr);  // BOTTOM
  frustumPlanes[2].update(ntl, nbl, fbl);  // LEFT
  frustumPlanes[3].update(nbr, ntr, fbr);  // RIGHT
  frustumPlanes[4].update(ntl, ntr, nbr);  // NEAR
  frustumPlanes[5].update(ftr, ftl, fbl);  // FAR
}

void Renderer3DFrustumPlanes::computeStaticData(const float& fov) {
  if (fabs(fov - lastFov) < 0.00001F) return;

  lastFov = fov;
  float tang = tanf(fov * Math::HALF_ANG2RAD);
  nearHeight = tang * settings->getNear();
  nearWidth = nearHeight * settings->getAspectRatio();
  farHeight = tang * settings->getFar();
  farWidth = farHeight * settings->getAspectRatio();
}

void Renderer3DFrustumPlanes::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void Renderer3DFrustumPlanes::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string Renderer3DFrustumPlanes::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "Renderer3DFrustumPlanes(";
  }
  res << std::fixed << std::setprecision(2);
  res << std::endl;

  res << frustumPlanes[0].getPrint("Top") << std::endl;
  res << frustumPlanes[1].getPrint("Bottom") << std::endl;
  res << frustumPlanes[2].getPrint("Left") << std::endl;
  res << frustumPlanes[3].getPrint("Right") << std::endl;
  res << frustumPlanes[4].getPrint("Near") << std::endl;
  res << frustumPlanes[5].getPrint("Far") << ")";

  return res.str();
}

}  // namespace Tyra
