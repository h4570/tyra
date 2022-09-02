
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "math/m4x4.hpp"
#include "renderer/3d/mesh/dynamic/dynamic_mesh_animation.hpp"
#include "debug/debug.hpp"

namespace Tyra {

DynamicMeshAnimation::DynamicMeshAnimation() {
  callback = nullptr;
  currentFrameSequenceIndex = 0;
  framesCount = 0;
}

DynamicMeshAnimation::~DynamicMeshAnimation() {}

void DynamicMeshAnimation::resetAll(const std::vector<MeshFrame*>& frames) {
  framesCount = frames.size();
  speed = 0.1F;
  loop = true;

  state.interpolation = 0.0F;
  state.currentFrame = 0;
  state.nextFrame = 0;

  sequence.clear();
  for (std::size_t i = 0; i < frames.size(); i++) {
    sequence.push_back(i);
  }

  restart();
}

void DynamicMeshAnimation::setSequence(
    const std::vector<unsigned int>& t_sequence) {
  TYRA_ASSERT(framesCount != 1,
              "Cant set sequence, because this mesh have only one frame.");

  TYRA_ASSERT(sequence.size() > 0,
              "Cant set sequence, because sequence is empty.");

  for (std::size_t i = 0; i < t_sequence.size(); i++) {
    TYRA_ASSERT(t_sequence[i] < framesCount && t_sequence[i] >= 0,
                "Cant set sequence, because value: ", t_sequence[i],
                " At index: ", i, " Is out of range!");
  }

  sequence = t_sequence;

  restart();
}

const DynamicMeshAnimState& DynamicMeshAnimation::getState() const {
  return state;
}

void DynamicMeshAnimation::update() {
  AnimationSequenceCallback callbackInfo =
      AnimationSequenceCallback::AnimationSequenceCallback_NextFrame;
  bool sendCallback = true;

  state.interpolation += speed;

  if (state.interpolation >= 1.0F) {
    state.interpolation = 0.0F;

    if (loop) {
      updateLoopState(&callbackInfo);
    } else {
      auto anythingChanged = updateNoLoopState(&callbackInfo);
      if (!anythingChanged) {
        sendCallback = false;
      }
    }
  }

  if (callback != nullptr && sendCallback) callback(callbackInfo);
}

void DynamicMeshAnimation::restart() {
  currentFrameSequenceIndex = 0;

  auto firstIndex = sequence[0];
  auto secondIndex = sequence.size() > 1 ? sequence[1] : firstIndex;

  state.currentFrame = sequence[firstIndex];
  state.nextFrame = sequence[secondIndex];
}

void DynamicMeshAnimation::updateLoopState(
    AnimationSequenceCallback* callbackInfo) {
  auto n1SequenceIndex = getNextSequenceIndexFrom(currentFrameSequenceIndex);

  state.currentFrame = state.nextFrame;

  auto isN1SequenceIndexEndOfSequence = n1SequenceIndex == sequence.size() - 1;
  if (isN1SequenceIndexEndOfSequence) {
    *callbackInfo = AnimationSequenceCallback::AnimationSequenceCallback_Loop;
  }

  state.nextFrame = sequence[getNextSequenceIndexFrom(n1SequenceIndex)];
  currentFrameSequenceIndex = n1SequenceIndex;
}

bool DynamicMeshAnimation::updateNoLoopState(
    AnimationSequenceCallback* callbackInfo) {
  if (state.currentFrame == state.nextFrame) {
    return false;  // Animation is finished.
  }

  auto n1SequenceIndex = getNextSequenceIndexFrom(currentFrameSequenceIndex);

  state.currentFrame = state.nextFrame;

  auto isN1SequenceIndexEndOfSequence = n1SequenceIndex == sequence.size() - 1;
  if (isN1SequenceIndexEndOfSequence) {
    state.nextFrame = sequence[sequence.size() - 1];
    *callbackInfo = AnimationSequenceCallback::AnimationSequenceCallback_End;
    currentFrameSequenceIndex = sequence.size() - 1;
  } else {
    state.nextFrame = sequence[getNextSequenceIndexFrom(n1SequenceIndex)];
    currentFrameSequenceIndex = n1SequenceIndex;
  }

  return true;
}

unsigned int DynamicMeshAnimation::getNextSequenceIndexFrom(
    const unsigned int& index) const {
  return index == sequence.size() - 1 ? 0 : index + 1;
}

void DynamicMeshAnimation::setCallback(
    const std::function<void(const AnimationSequenceCallback&)>& t_callback) {
  callback = t_callback;
}

}  // namespace Tyra
