/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "debug/debug.hpp"
#include "renderer/3d/pipeline/shared/bag/pipeline_dir_lights_bag.hpp"

namespace Tyra {

PipelineDirLightsBag::PipelineDirLightsBag(const bool& manual) {
  isAllocated = false;
  mode = Auto;
  lightColors = nullptr;
  lightDirections = nullptr;
  if (manual) {
    mode = Manual;
  } else {
    allocate();
  }
}

PipelineDirLightsBag::~PipelineDirLightsBag() { deallocate(); }

void PipelineDirLightsBag::setAmbientColor(const Color& color) {
  TYRA_ASSERT(mode != Manual, "Ambient color cannot be set in manual mode");
  lightColors[3].set(reinterpret_cast<const Vec4&>(color));
}

void PipelineDirLightsBag::setDirectionalLightColors(Color* colors,
                                                     const u8& count) {
  for (u8 i = 0; i < count; i++) setDirectionalLightColor(colors[i], i);
}

void PipelineDirLightsBag::setDirectionalLightDirections(Vec4* directions,
                                                         const u8& count) {
  for (u8 i = 0; i < count; i++) setDirectionalLightDirection(directions[i], i);
}

void PipelineDirLightsBag::setDirectionalLightColor(const Color& color,
                                                    const u8& index) {
  TYRA_ASSERT(mode != Manual,
              "Directional lights cannot be set in manual mode");
  TYRA_ASSERT(index < 3, "There are max 3 directional lights");
  lightColors[index].set(reinterpret_cast<const Vec4&>(color));
}

void PipelineDirLightsBag::setDirectionalLightDirection(const Vec4& direction,
                                                        const u8& index) {
  TYRA_ASSERT(mode != Manual,
              "Directional lights cannot be set in manual mode");
  TYRA_ASSERT(index < 3, "There are max 3 directional lights");

  lightDirections[index].set(direction);
}

void PipelineDirLightsBag::setLightsManually(Vec4* colors, Vec4* directions) {
  deallocate();
  lightColors = colors;
  lightDirections = directions;
  mode = Manual;
}

void PipelineDirLightsBag::disableManualMode() {
  allocate();
  mode = Auto;
}

void PipelineDirLightsBag::allocate() {
  if (isAllocated) return;

  lightColors = new Vec4[4];
  lightDirections = new Vec4[3];

  for (u8 i = 0; i < 3; i++) {
    lightColors[i].set(0.0F, 0.0F, 0.0F, 1.0F);
  }

  lightColors[3].set(.5F, .5F, .5F, 1.0F);  // Ambient

  for (u8 i = 0; i < 3; i++) {
    lightDirections[i].set(1.0F, 1.0F, 1.0F, 1.0F);
  }

  isAllocated = true;
}

void PipelineDirLightsBag::deallocate() {
  if (!isAllocated) return;

  forceDeallocate();
}

void PipelineDirLightsBag::forceDeallocate() {
  forceDeallocateColors();
  forceDeallocateDirections();

  isAllocated = false;
}

void PipelineDirLightsBag::forceDeallocateColors() {
  if (lightColors != nullptr) {
    delete[] lightColors;
    lightColors = nullptr;
  }
}

void PipelineDirLightsBag::forceDeallocateDirections() {
  if (lightDirections != nullptr) {
    delete[] lightDirections;
    lightDirections = nullptr;
  }
}

}  // namespace Tyra
