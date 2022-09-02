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

#include "./bag/packaging/stapip_bag_packages_bbox.hpp"
#include "shared/renderer/3d/mesh/mesh.hpp"
#include <memory>
#include <vector>

namespace Tyra {

struct StapipBagBBoxesCacheItem {
  unsigned int vu1MaxVertCount;
  unsigned int id;
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

  StaPipBagPackagesBBox* getBBoxes(const Vec4* vertices,
                                   const unsigned int& count,
                                   const unsigned int& id,
                                   const unsigned int& maxVertCount);

 private:
  StapipBagBBoxesCacheItem* getCache(const unsigned int& maxVertCount,
                                     const unsigned int& id);

  std::vector<StapipBagBBoxesCacheItem> storage;
};

}  // namespace Tyra
