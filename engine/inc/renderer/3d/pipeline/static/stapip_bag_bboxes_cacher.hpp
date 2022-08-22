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

#include <tamtypes.h>
#include "core/bag/packaging/stapip_bag_packages_bbox.hpp"
#include "renderer/3d/mesh/mesh.hpp"
#include <memory>
#include <vector>

namespace Tyra {

struct StapipBagBBoxesCacheItem {
  u32 vu1MaxVertCount;
  u32 frameId;
  std::unique_ptr<StaPipBagPackagesBBox> bboxes;
  int framesLeftToDestroy;
};

class StapipBagBBoxesCacher {
 public:
  StapipBagBBoxesCacher();
  ~StapipBagBBoxesCacher();

  const int cacheFramesCount = 50;
  const int cacheSecondsCount = 5;

  void onFrameEnd();

  StaPipBagPackagesBBox* getBBoxesByMesh(const MeshMaterialFrame* frame,
                                         const u32& maxVertCount);

 private:
  StapipBagBBoxesCacheItem* getCache(const u32& maxVertCount,
                                     const u32& frameId);

  std::vector<StapipBagBBoxesCacheItem> storage;
};

}  // namespace Tyra
