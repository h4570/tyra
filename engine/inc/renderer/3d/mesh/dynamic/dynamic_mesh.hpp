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

#include "../mesh_frame.hpp"
#include "../mesh_material_frame.hpp"
#include "./dynamic_mesh_anim_state.hpp"
#include "../mesh.hpp"

namespace Tyra {

class DynamicMesh : public Mesh {
 public:
  explicit DynamicMesh(const MeshBuilderData& data);
  explicit DynamicMesh(const DynamicMesh& mesh);
  ~DynamicMesh();

  std::vector<MeshFrame*> frames;

  DynamicMeshAnimState animState;

  /** @returns bounding box object of current frame. */
  const BBox& getCurrentBoundingBox() const {
    return *frames[animState.currentFrame]->bbox;
  }

  /** Loop in one frame */
  void playAnimation(const u32& t_frame);

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
};

}  // namespace Tyra
