/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/renderer/3d/pipeline/static/core/stapip_bag_bboxes_cacher.hpp"
#include <algorithm>

namespace Tyra {

StapipBagBBoxesCacher::StapipBagBBoxesCacher() {}

StapipBagBBoxesCacher::~StapipBagBBoxesCacher() {}

void StapipBagBBoxesCacher::onFrameEnd() {
  for (auto& item : storage) {
    if (item.framesLeftToDestroy > 0) {
      item.framesLeftToDestroy--;
    }
  }

  storage.erase(std::remove_if(storage.begin(), storage.end(),
                               [](const StapipBagBBoxesCacheItem& item) {
                                 return item.framesLeftToDestroy <= 0;
                               }),
                storage.end());
}

StaPipBagPackagesBBox* StapipBagBBoxesCacher::getBBoxes(
    const Vec4* vertices, const unsigned int& count, const unsigned int& id,
    const unsigned int& maxVertCount) {
  auto* cache = getCache(maxVertCount, id);

  if (cache) {
    cache->framesLeftToDestroy = cacheFramesCount * cacheSecondsCount;
    return cache->bboxes.get();
  }

  auto bboxes =
      std::make_unique<StaPipBagPackagesBBox>(vertices, count, maxVertCount);

  storage.push_back(
      StapipBagBBoxesCacheItem{maxVertCount, id, std::move(bboxes),
                               cacheFramesCount * cacheSecondsCount});

  return storage.back().bboxes.get();
}

StapipBagBBoxesCacheItem* StapipBagBBoxesCacher::getCache(
    const unsigned int& maxVertCount, const unsigned int& id) {
  for (auto& item : storage) {
    if (item.vu1MaxVertCount == maxVertCount && item.id == id) {
      return &item;
    }
  }

  return nullptr;
}

}  // namespace Tyra
