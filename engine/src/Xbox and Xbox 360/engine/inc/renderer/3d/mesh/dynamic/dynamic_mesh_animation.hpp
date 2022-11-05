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
  void setSequence(const std::vector<u32>& sequence);

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
  u32 framesCount;
  u32 currentFrameSequenceIndex;
  DynamicMeshAnimState state;

  std::function<void(const AnimationSequenceCallback&)> callback;
  std::vector<u32> sequence;

  void updateLoopState(AnimationSequenceCallback* callbackInfo);

  /**
   * @return true Frame changed
   * @return false No changes
   */
  bool updateNoLoopState(AnimationSequenceCallback* callbackInfo);

  u32 getNextSequenceIndexFrom(const u32& index) const;
};

}  // namespace Tyra
