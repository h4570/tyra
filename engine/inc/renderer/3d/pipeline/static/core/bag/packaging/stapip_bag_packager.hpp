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

#include "renderer/core/3d/bbox/core_bbox.hpp"
#include "renderer/core/3d/renderer_3d_frustum_planes.hpp"
#include "renderer/core/3d/clipper/planes_clip_algorithm.hpp"
#include "./stapip_bag_packages_bbox.hpp"
#include "./stapip_bag_package.hpp"
#include "../stapip_bag.hpp"

namespace Tyra {

class StaPipBagPackager {
 public:
  StaPipBagPackager();
  ~StaPipBagPackager();

  void init(Renderer3DFrustumPlanes* frustumPlanes);
  void setRenderBBox(StaPipBagPackagesBBox* bbox) { renderBBox = bbox; }
  void setMaxVertCount(const unsigned int& count);

  /**
   * @brief Create render packages from provided render data
   *
   * @param size Max maxVertCount verts (VU1 buffer size)
   */
  StaPipBagPackage* create(unsigned short* o_size, StaPipBag* data,
                           unsigned short size);
  /**
   * @brief Split render package to smaller packages
   *
   * @param size Max maxVertCount verts (VU1 buffer size)
   */
  StaPipBagPackage* create(unsigned short* o_size, const StaPipBagPackage& pkg,
                           unsigned short size);

  CoreBBoxFrustum checkFrustum(const StaPipBagPackage& pkg);

 private:
  unsigned int maxVertCount;
  Renderer3DFrustumPlanes* frustumPlanes;
  StaPipBagPackagesBBox* renderBBox;
};

}  // namespace Tyra
