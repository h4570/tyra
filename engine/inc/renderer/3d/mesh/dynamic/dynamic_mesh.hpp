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

#include "../mesh_frame.hpp"
#include "../mesh_material_frame.hpp"
#include "./dynamic_mesh_anim_state.hpp"
#include "../mesh.hpp"

namespace Tyra {

class DynamicMesh : public Mesh {
 public:
  explicit DynamicMesh(const MeshBuilder2Data& data);
  explicit DynamicMesh(const DynamicMesh& mesh);
  ~DynamicMesh();

  const DynamicMeshAnimState& getAnimState() const { return animState; }

  /** Returns single frame. */
  MeshFrame* getFrame(const u32& i) { return frames[i]; }

  MeshFrame** getFrames() { return frames; }

  /** Count of frames. Static object (not animated) will have only 1 frame. */
  const u32& getFramesCount() const { return framesCount; }

  const u32& getCurrentAnimationFrame() const { return animState.currentFrame; }
  const u32& getNextAnimationFrame() const { return animState.nextFrame; }
  const u32& getStartAnimationFrame() const { return animState.startFrame; }
  const u32& getEndAnimationFrame() const { return animState.endFrame; }
  const u32& getStayAnimationFrame() const { return animState.stayFrame; }

  void setCurrentAnimationFrame(const u32& v) { animState.currentFrame = v; }
  void setNextAnimationFrame(const u32& v) { animState.nextFrame = v; }
  void setStartAnimationFrame(const u32& v) { animState.startFrame = v; }
  void setEndAnimationFrame(const u32& v) { animState.endFrame = v; }
  void setStayAnimationFrame(const u32& v) { animState.stayFrame = v; }

  /** @returns bounding box object of current frame. */
  const BBox& getCurrentBoundingBox() const {
    return frames[animState.currentFrame]->getBBox();
  }

  /** Loop in one frame */
  void playAnimation(const u32& t_frame) { playAnimation(t_frame, t_frame); }

  /** Play animation in loop from startFrame to endFrame */
  void playAnimation(const u32& t_startFrame, const u32& t_endFrame);

  /** Play animation from startFrame to endFrame and after loop in stayFrame
   */
  void playAnimation(const u32& t_startFrame, const u32& t_endFrame,
                     const u32& t_stayFrame);

  void setAnimSpeed(const float& t_value) { animState.speed = t_value; }

  void animate();

  /**
   * Check if this class loaded mesh data first.
   * Meshes which use loadFrom() method have false there.
   */
  const u8& isStayAnimationSet() const { return animState.isStayFrameSet; }

 private:
  void initAnimation();
  DynamicMeshAnimState animState;
  u32 framesCount;
  MeshFrame** frames;
};

}  // namespace Tyra
