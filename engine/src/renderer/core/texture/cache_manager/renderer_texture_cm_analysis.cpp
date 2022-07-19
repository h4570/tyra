/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/texture/cache_manager/renderer_texture_cm_analysis.hpp"

namespace Tyra {

RendererTextureCMAnalysis::RendererTextureCMAnalysis() {
  last = &buffers[0];
  current = &buffers[1];
  currentIndex = 0;
}
RendererTextureCMAnalysis::~RendererTextureCMAnalysis() {}

void RendererTextureCMAnalysis::changeFrame() {
  flipBuffers();
  current->clear();
  currentIndex = 0;
}

void RendererTextureCMAnalysis::addNewRequest(const u32& id) {
  current->push_back(id);
  currentIndex++;
}

std::vector<u32> RendererTextureCMAnalysis::getTopTextureIdsForNextChanges(
    const u8& count) {
  TYRA_ASSERT(!isProbablyNotCorrect(),
              "Analysis not available, because its probably not correct - "
              "internal error");

  auto nextTextureIds = getNextTextureIds(count);
  auto occurences = getOccurencesForTextureIds(nextTextureIds);
  auto uniqueTextureIds = getTextureIdsUnique(nextTextureIds);
  auto weights = getWeights(uniqueTextureIds, occurences);
  auto result = weightsToResult(&weights);

  return result;
}

std::vector<u32> RendererTextureCMAnalysis::getNextTextureIds(const u8& count) {
  std::vector<u32> result;

  for (u32 i = 0; i < count; i++) {
    u32 index = i + currentIndex;
    u32 offset = index >= last->size() ? index - last->size() : index;
    result.push_back(last->at(offset));
  }

  return result;
}

std::vector<RendererTextureCMAnalysisOccurence>
RendererTextureCMAnalysis::getOccurencesForTextureIds(
    const std::vector<u32>& textureIds) {
  std::vector<RendererTextureCMAnalysisOccurence> result;

  for (u8 i = 0; i < textureIds.size(); i++) {
    // If not exist, add with 1 occurence
    if (std::find_if(result.begin(), result.end(),
                     [textureIds,
                      i](const RendererTextureCMAnalysisOccurence& occurence) {
                       return occurence.id == textureIds[i];
                     }) == result.end()) {
      result.push_back({textureIds[i], 1});
    } else {
      // If exist, increase occurence
      auto occurence = std::find_if(
          result.begin(), result.end(),
          [textureIds, i](const RendererTextureCMAnalysisOccurence& occurence) {
            return occurence.id == textureIds[i];
          });
      occurence->occurence++;
    }
  }

  return result;
}

std::vector<u32> RendererTextureCMAnalysis::getTextureIdsUnique(
    const std::vector<u32>& textureIds) {
  std::vector<u32> result;

  for (u8 i = 0; i < textureIds.size(); i++) {
    if (std::find(result.begin(), result.end(), textureIds[i]) ==
        result.end()) {
      result.push_back(textureIds[i]);
    }
  }

  return result;
}

std::vector<RendererTextureCMAnalysisWeight>
RendererTextureCMAnalysis::getWeights(
    const std::vector<u32>& uniqueTexIds,
    const std::vector<RendererTextureCMAnalysisOccurence>& occurencies) {
  std::vector<RendererTextureCMAnalysisWeight> result;

  for (u8 i = 0; i < uniqueTexIds.size(); i++) {
    u16 weight = uniqueTexIds.size() - i;

    for (u8 j = 0; j < occurencies.size(); j++) {
      if (occurencies[j].id == uniqueTexIds[i]) {
        weight *= occurencies[j].occurence;
        break;
      }
    }

    result.push_back({uniqueTexIds[i], weight});
  }

  return result;
}

std::vector<u32> RendererTextureCMAnalysis::weightsToResult(
    std::vector<RendererTextureCMAnalysisWeight>* weights) {
  std::sort(weights->begin(), weights->end(),
            [](const auto& a, const auto& b) { return a.weight > b.weight; });

  std::vector<u32> result;
  for (u8 i = 0; i < weights->size(); i++) {
    result.push_back(weights->at(i).id);
  }

  return result;
}

bool RendererTextureCMAnalysis::isReady() { return last->size() != 0; }

bool RendererTextureCMAnalysis::isProbablyNotCorrect() {
  return currentIndex > last->size();
}

void RendererTextureCMAnalysis::flipBuffers() {
  if (current == &buffers[0]) {
    current = &buffers[1];
    last = &buffers[0];
  } else {
    current = &buffers[0];
    last = &buffers[1];
  }
}

}  // namespace Tyra
