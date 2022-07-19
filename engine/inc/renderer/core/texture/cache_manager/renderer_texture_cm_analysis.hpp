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

#include <algorithm>
#include "debug/debug.hpp"
#include <tamtypes.h>
#include <vector>

namespace Tyra {

struct RendererTextureCMAnalysisWeight {
  u32 id;
  u16 weight;
};

struct RendererTextureCMAnalysisOccurence {
  u32 id;
  u8 occurence;
};

class RendererTextureCMAnalysis {
 public:
  RendererTextureCMAnalysis();
  ~RendererTextureCMAnalysis();

  void changeFrame();

  void addNewRequest(const u32& id);

  /**
   * @brief Returns top list (from most used to least used) of textures to be
   * deallocated.
   *
   * @param count Number of textures to be returned.
   */
  std::vector<u32> getTopTextureIdsForNextChanges(const u8& count);

  bool isReady();

  bool isProbablyNotCorrect();

 private:
  std::vector<u32>*current, *last;
  std::vector<u32> buffers[2];
  u32 currentIndex;

  void flipBuffers();

  std::vector<u32> getNextTextureIds(const u8& count);

  std::vector<RendererTextureCMAnalysisOccurence> getOccurencesForTextureIds(
      const std::vector<u32>& textureIds);

  std::vector<u32> getTextureIdsUnique(const std::vector<u32>& textureIds);

  std::vector<RendererTextureCMAnalysisWeight> getWeights(
      const std::vector<u32>& uniqueTexIds,
      const std::vector<RendererTextureCMAnalysisOccurence>& occurencies);

  std::vector<u32> weightsToResult(
      std::vector<RendererTextureCMAnalysisWeight>* weights);
};

}  // namespace Tyra
