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

#include <tamtypes.h>
#include "./bag/stdpip_bag.hpp"
#include "./bag/packaging/stdpip_bag_packages_bbox.hpp"
#include "./bag/packaging/stdpip_bag_package.hpp"
#include "./bag/packaging/stdpip_bag_packager.hpp"
#include "./path1/stdpip_qbuffer_renderer.hpp"

namespace Tyra {

class StdPipelineCore {
 public:
  StdPipelineCore();
  ~StdPipelineCore();

  void init(RendererCore* t_core);

  /** Render 3D data via "bags" */
  void render(StdpipBag* data, StdpipBagPackagesBBox* bbox = nullptr);

  /** Get max vert count of VU1 qbuffer (for optimizations) */
  u32 getMaxVertCountByParams(const bool& isSingleColor,
                              const bool& isLightingEnabled,
                              const bool& isTextureEnabled);

  /** Get max vert count of VU1 qbuffer (for optimizations) */
  u32 getMaxVertCountByBag(const StdpipBag* bag);

  /**
   * - Uploads standard VU1 programs.
   * - Sends static "Tyra Renderer3D" VU1 data.
   * - Sets double buffers exactly for "Tyra Renderer3D"
   * Should be called if VU1 was used by your non standard programs.
   */
  void reinitStandardVU1Programs();

 private:
  u32 maxVertCount;
  RendererCore* rendererCore;

  void setMaxVertCount(const u32& count);
  StdpipBagPackager packager;
  StdpipQBufferRenderer qbufferRenderer;
  void renderPkgs(StdpipBagPackage* packages, u16 count);
  void renderSubpkgs(StdpipBagPackage* packages, u16 count);
};

}  // namespace Tyra
