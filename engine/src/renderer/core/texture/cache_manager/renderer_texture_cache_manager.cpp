/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/texture/cache_manager/renderer_texture_cache_manager.hpp"

namespace Tyra {

RendererTextureCacheManager::RendererTextureCacheManager() {}
RendererTextureCacheManager::~RendererTextureCacheManager() {}

void RendererTextureCacheManager::onFrameChange() { statistics.changeFrame(); }

void RendererTextureCacheManager::addRequestedTexture(Texture* texture) {
  statistics.addNewRequest(texture->getId());
}

u32 RendererTextureCacheManager::getTextureIdToDealloc(
    const std::vector<RendererCoreTextureBuffers>& currentAllocs) {
  if (!statistics.isReady() || statistics.isProbablyNotCorrect()) {
    return currentAllocs.size() > 0 ? currentAllocs.at(0).id : 0;
  }

  u8 tuner = currentAllocs.size() > 254 ? 254 : currentAllocs.size();
  if (tuner >= 6) {
    tuner /= 3;
  } else if (tuner >= 4) {
    tuner /= 2;
  } else {
    tuner = 1;
  }

  std::vector<u32> temp;

  auto probableTextureIds = statistics.getTopTextureIdsForNextChanges(tuner);

  tryAddIdThatNotExistInAnalysis(&temp, currentAllocs, probableTextureIds);

  if (temp.size() == 0) {
    addIdThatExistInAnalysisButIsOnTheBottom(&temp, currentAllocs,
                                             probableTextureIds);
  }

  return temp.at(0);
}

void RendererTextureCacheManager::tryAddIdThatNotExistInAnalysis(
    std::vector<u32>* result,
    const std::vector<RendererCoreTextureBuffers>& currentAllocs,
    const std::vector<u32>& analysis) {
  for (u32 i = 0; i < currentAllocs.size(); i++) {
    if (std::find(analysis.begin(), analysis.end(), currentAllocs.at(i).id) ==
        analysis.end()) {
      result->push_back(currentAllocs.at(i).id);
      break;
    }
  }
}

void RendererTextureCacheManager::addIdThatExistInAnalysisButIsOnTheBottom(
    std::vector<u32>* result,
    const std::vector<RendererCoreTextureBuffers>& currentAllocs,
    const std::vector<u32>& analysis) {
  u32 lastId = analysis.at(analysis.size() - 1);
  result->push_back(lastId);
}

}  // namespace Tyra
