/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "debug/debug.hpp"
#include "renderer/3d/pipeline/shared/bag/pipeline_lighting_bag.hpp"

namespace Tyra {

PipelineLightingBag::PipelineLightingBag(const bool& manual) {
  isAllocated = false;
  mode = Auto;
  normals = nullptr;
  lightMatrix = nullptr;
  lightColors = nullptr;
  lightDirections = nullptr;
  if (manual) {
    mode = Manual;
  } else {
    allocate();
  }
}

PipelineLightingBag::~PipelineLightingBag() { deallocate(); }

void PipelineLightingBag::setAmbientColor(const Color& color) {
  TYRA_ASSERT(mode != Manual, "Ambient color cannot be set in manual mode");
  lightColors[3].set(reinterpret_cast<const Vec4&>(color));
}

void PipelineLightingBag::setDirectionalLightColors(Color* colors,
                                                    const u8& count) {
  for (u8 i = 0; i < count; i++) setDirectionalLightColor(colors[i], i);
}

void PipelineLightingBag::setDirectionalLightDirections(Vec4* directions,
                                                        const u8& count) {
  for (u8 i = 0; i < count; i++) setDirectionalLightDirection(directions[i], i);
}

void PipelineLightingBag::setDirectionalLightColor(const Color& color,
                                                   const u8& index) {
  TYRA_ASSERT(mode != Manual,
              "Directional lights cannot be set in manual mode");
  TYRA_ASSERT(index < 3, "There are max 3 directional lights");
  lightColors[index].set(reinterpret_cast<const Vec4&>(color));
}

void PipelineLightingBag::setDirectionalLightDirection(const Vec4& direction,
                                                       const u8& index) {
  TYRA_ASSERT(mode != Manual,
              "Directional lights cannot be set in manual mode");
  TYRA_ASSERT(index < 3, "There are max 3 directional lights");

  lightDirections[index].set(direction);
}

void PipelineLightingBag::setLightsManually(Vec4* colors, Vec4* directions) {
  deallocate();
  lightColors = colors;
  lightDirections = directions;
  mode = Manual;
}

void PipelineLightingBag::disableManualMode() {
  allocate();
  mode = Auto;
}

void PipelineLightingBag::allocate() {
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

void PipelineLightingBag::deallocate() {
  if (!isAllocated) return;

  forceDeallocate();
}

void PipelineLightingBag::forceDeallocate() {
  forceDeallocateColors();
  forceDeallocateDirections();

  isAllocated = false;
}

void PipelineLightingBag::forceDeallocateColors() {
  if (lightColors != nullptr) {
    delete[] lightColors;
    lightColors = nullptr;
  }
}

void PipelineLightingBag::forceDeallocateDirections() {
  if (lightDirections != nullptr) {
    delete[] lightDirections;
    lightDirections = nullptr;
  }
}

}  // namespace Tyra
