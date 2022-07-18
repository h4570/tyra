
/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tamtypes.h>
#include "math/m4x4.hpp"
#include "renderer/3d/mesh/dynamic/dynamic_mesh.hpp"

namespace Tyra {

DynamicMesh::DynamicMesh(const MeshBuilderData& data) {
  framesCount = data.framesCount;
  TYRA_ASSERT(framesCount > 0, "Frames count must be greater than 0");

  materialsCount = data.materialsCount;
  TYRA_ASSERT(materialsCount > 0, "Materials count must be greater than 0");

  frames = new DynamicMeshFrame*[framesCount];
  for (u32 i = 0; i < framesCount; i++) {
    frames[i] = new DynamicMeshFrame(data, i);
  }

  materials = new DynamicMeshMaterial*[materialsCount];
  for (u32 i = 0; i < materialsCount; i++) {
    materials[i] = new DynamicMeshMaterial(data, i);
  }

  translation.translate(Vec4(0.0F, 0.0F, 0.0F, 1.0F));

  initMesh();

  _isMother = true;
}

DynamicMesh::DynamicMesh(const DynamicMesh& mesh) {
  framesCount = mesh.framesCount;
  materialsCount = mesh.materialsCount;

  frames = new DynamicMeshFrame*[framesCount];
  for (u32 i = 0; i < framesCount; i++) {
    frames[i] = new DynamicMeshFrame(*mesh.frames[i]);
  }

  materials = new DynamicMeshMaterial*[materialsCount];
  for (u32 i = 0; i < materialsCount; i++) {
    materials[i] = new DynamicMeshMaterial(*mesh.materials[i]);
  }

  translation.translate(Vec4(0.0F, 0.0F, 0.0F, 1.0F));

  initMesh();

  _isMother = false;
}

DynamicMesh::~DynamicMesh() {
  for (u32 i = 0; i < framesCount; i++) {
    delete frames[i];
  }
  delete[] frames;

  for (u32 i = 0; i < materialsCount; i++) {
    delete materials[i];

    delete[] materials;
  }
}

M4x4 DynamicMesh::getModelMatrix() const {
  return translation * rotation * scale;
}

void DynamicMesh::initMesh() {
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

void DynamicMesh::playAnimation(const u32& t_startFrame,
                                const u32& t_endFrame) {
  TYRA_ASSERT(framesCount > 0,
              "Cant play animation, because no mesh data was loaded!");
  TYRA_ASSERT(framesCount != 1,
              "Cant play animation, because this mesh have only one frame.");
  TYRA_ASSERT(
      t_endFrame < framesCount,
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
  TYRA_ASSERT(framesCount > 0,
              "Cant play animation, because no mesh data was loaded!");
  TYRA_ASSERT(framesCount != 1,
              "Cant play animation, because this mesh have only one frame.");
  TYRA_ASSERT(
      t_endFrame < framesCount,
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
