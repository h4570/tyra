/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "states/game/terrain/heightmap.hpp"

using Tyra::FileUtils;
using Tyra::PngLoader;
using Tyra::PngPixel3;
using Tyra::Vec4;

namespace Demo {

Heightmap::Heightmap(const float& t_minHeight, const float& t_maxHeight,
                     const Vec4& t_leftUp, const Vec4& t_rightDown)
    : minHeight(t_minHeight),
      maxHeight(t_maxHeight),
      leftUp(t_leftUp),
      rightDown(t_rightDown) {
  PngLoader loader;
  auto* data =
      loader.load(FileUtils::fromCwd("game/models/terrain/heightmap.png"));

  allocateMap(data->data, data->width, data->height);

  delete data;
}

bool Heightmap::isOutside(const Vec4& position) const {
  return position.x <= leftUp.x || position.x >= rightDown.x ||
         position.z <= leftUp.z || position.z >= rightDown.z;
}

Heightmap::~Heightmap() {
  if (map) {
    for (int i = 0; i < mapHeight; i++) {
      delete[] map[i];
    }
    delete[] map;
  }
}

const float& Heightmap::getHeightOffset(const Vec4& playerPosition) const {
  float widthPercentage = getWidthPercentage(playerPosition);
  float heightPercentage = getHeightPercentage(playerPosition);

  auto x = static_cast<s16>(widthPercentage * mapWidth);
  auto y = static_cast<s16>(heightPercentage * mapHeight);

  TYRA_ASSERT(x >= 0 && x < mapWidth, "x is out of range");
  TYRA_ASSERT(y >= 0 && y < mapHeight, "y is out of range");

  return map[x][y];
}

float Heightmap::getWidthPercentage(const Vec4& playerPosition) const {
  return (playerPosition.x - leftUp.x) / (rightDown.x - leftUp.x);
}

float Heightmap::getHeightPercentage(const Vec4& playerPosition) const {
  return (playerPosition.z - leftUp.z) / (rightDown.z - leftUp.z);
}

void Heightmap::allocateMap(const unsigned char* data, const int& width,
                            const int& height) {
  auto* pixels = (struct PngPixel3*)data;

  u8 min = pixels[0].r;  // in grayscale every r,g,b is the same
  u8 max = pixels[0].r;

  // Get min and max
  for (int i = 0; i < width * height; i++) {
    const auto& value = pixels[i].r;
    if (value < min) {
      min = value;
    }
    if (value > max) {
      max = value;
    }
  }

  // Allocate
  mapWidth = width;
  mapHeight = height;
  map = new float*[width];
  for (int i = 0; i < width; i++) {
    map[i] = new float[height];
  }

  // Fill
  int k = 0;
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      map[i][j] = getGameHeight(pixels[k++].r, min, max);
    }
  }
}

float Heightmap::getGameHeight(const u8& inputColor, const u8& minColor,
                               const u8& maxColor) const {
  const float mapRange = maxColor - minColor;
  const float gameRange = maxHeight - minHeight;

  return (inputColor - minColor) / mapRange * gameRange + minHeight;
}

}  // namespace Demo
