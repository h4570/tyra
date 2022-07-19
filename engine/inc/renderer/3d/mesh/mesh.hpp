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

#include "loaders/3d/builder/mesh_builder_data.hpp"
#include "./mesh_frame.hpp"
#include "./mesh_material_frame.hpp"
#include "./mesh_anim_state.hpp"

namespace Tyra {

class Mesh {
 public:
  explicit Mesh(const MeshBuilderData& data);
  explicit Mesh(const Mesh& mesh);
  ~Mesh();

  /** Translation matrix */
  M4x4 translation;

  /** Rotation matrix */
  M4x4 rotation;

  /** Scale matrix */
  M4x4 scale;

  const u32& getId() const { return id; }

  const u8& isMother() const { return _isMother; }

  /** Get position from translation matrix */
  Vec4* getPosition() {
    return reinterpret_cast<Vec4*>(&translation.data[3 * 4]);
  }

  void setPosition(const Vec4& v) {
    TYRA_ASSERT(v.w == 1.0F, "Vec4 must be homogeneous");
    reinterpret_cast<Vec4*>(&translation.data[3 * 4])->set(v);
  }

  /** Returns material, which is a mesh "subgroup". */
  MeshMaterial* getMaterial(const u32& i) const { return materials[i]; }

  const u32& getMaterialsCount() const { return materialsCount; }

  const MeshAnimState& getAnimState() const { return animState; }

  /** Count of all vertices.  */
  u32 getVertexCount() { return frames[0]->getVertexCount(); }

  /** Returns single frame. */
  MeshFrame* getFrame(const u32& i) const { return frames[i]; }

  /** Count of frames. Static object (not animated) will have only 1 frame. */
  const u32& getFramesCount() const { return framesCount; }

  const u32& getCurrentAnimationFrame() const { return animState.currentFrame; }

  const u32& getNextAnimationFrame() const { return animState.nextFrame; }

  const u32& getStartAnimationFrame() const { return animState.startFrame; }

  const u32& getEndAnimationFrame() const { return animState.endFrame; }

  const u32& getStayAnimationFrame() const { return animState.stayFrame; }

  M4x4 getModelMatrix() const;

  // /**
  //  * Returns material, which is a mesh "subgroup".
  //  * NULL if not found.
  //  */
  // MeshMaterial* getMaterialById(const u32& t_id) const;

  /** @returns bounding box object of current frame. */
  const BBox& getCurrentBoundingBox() const {
    return frames[animState.currentFrame]->getBBox();
  }

  /** Check if are there any frames */
  u8 isDataLoaded() const { return framesCount > 0; }

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
  void initMesh();
  MeshAnimState animState;
  u8 _isMother;
  u32 id, framesCount, materialsCount;
  MeshFrame** frames;
  MeshMaterial** materials;
};

}  // namespace Tyra
