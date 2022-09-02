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

#include "./dynamic_mesh_anim_state.hpp"
#include "./animation_sequence_callback.hpp"
#include "../mesh_frame.hpp"
#include <vector>
#include <functional>

namespace Tyra {

class DynamicMeshAnimation {
 public:
  DynamicMeshAnimation();
  ~DynamicMeshAnimation();

  bool loop;
  float speed;

  /**
   * Update animation.
   * Should be called every frame if you want animation running.
   */
  void update();

  /** Set animation sequence (indices of frames) */
  void setSequence(const std::vector<unsigned int>& sequence);

  /** Start animation from beginning */
  void restart();

  /**
   * @brief Set animation callback.
   * It will push animation info on every update() call.
   */
  void setCallback(
      const std::function<void(const AnimationSequenceCallback&)>& callback);

  const DynamicMeshAnimState& getState() const;

  void resetAll(const std::vector<MeshFrame*>& frames);

 private:
  unsigned int framesCount;
  unsigned int currentFrameSequenceIndex;
  DynamicMeshAnimState state;

  std::function<void(const AnimationSequenceCallback&)> callback;
  std::vector<unsigned int> sequence;

  void updateLoopState(AnimationSequenceCallback* callbackInfo);

  /**
   * @return true Frame changed
   * @return false No changes
   */
  bool updateNoLoopState(AnimationSequenceCallback* callbackInfo);

  unsigned int getNextSequenceIndexFrom(const unsigned int& index) const;
};

}  // namespace Tyra
