
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tamtypes.h>
#include "math/m4x4.hpp"
#include "renderer/3d/mesh/dynamic/dynamic_mesh.hpp"

namespace Tyra {

DynamicMesh::DynamicMesh(const MeshBuilderData& data) : Mesh(data) {
  if (data.materials[0]->frames.size() == 1) {
    TYRA_WARN(
        "Frames count should be greater than 1 for DynamicMesh! Maybe you "
        "should use StaticMesh?");
  }

  for (u32 i = 0; i < data.materials[0]->frames.size(); i++) {
    frames.push_back(new MeshFrame(data, i));
  }

  initAnimation();
}

DynamicMesh::DynamicMesh(const DynamicMesh& mesh) : Mesh(mesh) {
  for (u32 i = 0; i < mesh.frames.size(); i++) {
    frames.push_back(new MeshFrame(*mesh.frames[i]));
  }

  initAnimation();
}

DynamicMesh::~DynamicMesh() {
  for (u32 i = 0; i < frames.size(); i++) {
    delete frames[i];
  }
}

void DynamicMesh::initAnimation() {
  animState.startFrame = 0;
  animState.endFrame = 0;
  animState.interpolation = 0.0F;
  animState.animType = 0;
  animState.currentFrame = 0;
  animState.stayFrame = 0;
  animState.isStayFrameSet = false;
  animState.nextFrame = 0;
  animState.speed = 0.1F;
}

void DynamicMesh::playAnimation(const u32& t_frame) {
  playAnimation(t_frame, t_frame);
}

void DynamicMesh::playAnimation(const u32& t_startFrame,
                                const u32& t_endFrame) {
  TYRA_ASSERT(frames.size() > 0,
              "Cant play animation, because no mesh data was loaded!");
  TYRA_ASSERT(frames.size() != 1,
              "Cant play animation, because this mesh have only one frame.");
  TYRA_ASSERT(
      t_endFrame < frames.size(),
      "End frame value is too high. Valid range: (0, getFramesCount()-1)");
  animState.startFrame = t_startFrame;
  animState.endFrame = t_endFrame;
  if (animState.currentFrame == t_startFrame)
    animState.nextFrame = t_endFrame;
  else
    animState.nextFrame = t_startFrame;
}

void DynamicMesh::playAnimation(const u32& t_startFrame, const u32& t_endFrame,
                                const u32& t_stayFrame) {
  TYRA_ASSERT(frames.size() > 0,
              "Cant play animation, because no mesh data was loaded!");
  TYRA_ASSERT(frames.size() != 1,
              "Cant play animation, because this mesh have only one frame.");
  TYRA_ASSERT(
      t_endFrame < frames.size(),
      "End frame value is too high. Valid range: (0, getFramesCount()-1)");
  animState.startFrame = t_startFrame;
  animState.endFrame = t_endFrame;
  animState.isStayFrameSet = true;
  animState.stayFrame = t_stayFrame;
  animState.nextFrame = t_startFrame;
}

void DynamicMesh::animate() {
  animState.interpolation += animState.speed;
  if (animState.interpolation >= 1.0F) {
    animState.interpolation = 0.0F;
    animState.currentFrame = animState.nextFrame;
    if (++animState.nextFrame > animState.endFrame) {
      if (animState.isStayFrameSet) {
        animState.isStayFrameSet = false;
        animState.nextFrame = animState.stayFrame;
        animState.startFrame = animState.stayFrame;
        animState.endFrame = animState.stayFrame;
      } else {
        animState.nextFrame = animState.startFrame;
      }
    }
  }
}

}  // namespace Tyra
