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

#include <string>
#include "renderer/renderer_settings.hpp"
#include "./camera_info_3d.hpp"
#include "math/plane.hpp"

namespace Tyra {

class Renderer3DFrustumPlanes {
 public:
  Renderer3DFrustumPlanes();
  ~Renderer3DFrustumPlanes();

  void init(RendererSettings* settings, const float& fov);
  void update(const CameraInfo3D& cameraInfo, const float& fov);
  const Plane& get(u8 index) const { return frustumPlanes[index]; }
  const Plane* getAll() const { return frustumPlanes; }
  const Plane& operator[](u8 index) const { return frustumPlanes[index]; }

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 private:
  void computeStaticData(const float& fov);
  float lastFov;
  RendererSettings* settings;
  Plane frustumPlanes[6];
  float nearHeight, nearWidth, farHeight, farWidth;
  Vec4 nearCenter, farCenter, X, Y, Z, ntl, ntr, nbl, nbr, ftl, fbr, ftr, fbl;
};

}  // namespace Tyra
