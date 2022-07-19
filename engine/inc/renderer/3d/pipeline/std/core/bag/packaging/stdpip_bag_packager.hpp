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

#include "renderer/core/3d/bbox/core_bbox.hpp"
#include "renderer/core/3d/renderer_3d_frustum_planes.hpp"
#include "renderer/core/paths/path1/clipper/path1_ee_clip_algorithm.hpp"
#include "./stdpip_bag_packages_bbox.hpp"
#include "./stdpip_bag_package.hpp"
#include "../stdpip_bag.hpp"

namespace Tyra {

class StdpipBagPackager {
 public:
  StdpipBagPackager();
  ~StdpipBagPackager();

  void init(Renderer3DFrustumPlanes* frustumPlanes);
  void setRenderBBox(StdpipBagPackagesBBox* bbox) { renderBBox = bbox; }
  void setMaxVertCount(const u32& count);

  /**
   * @brief Create render packages from provided render data
   *
   * @param size Max maxVertCount verts (VU1 buffer size)
   */
  StdpipBagPackage* create(u16* o_size, StdpipBag* data, u16 size);
  /**
   * @brief Split render package to smaller packages
   *
   * @param size Max maxVertCount verts (VU1 buffer size)
   */
  StdpipBagPackage* create(u16* o_size, const StdpipBagPackage& pkg, u16 size);

  CoreBBoxFrustum checkFrustum(const StdpipBagPackage& pkg);

 private:
  u32 maxVertCount;
  Renderer3DFrustumPlanes* frustumPlanes;
  StdpipBagPackagesBBox* renderBBox;
};

}  // namespace Tyra
