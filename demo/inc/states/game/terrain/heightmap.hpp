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

#include <tyra>

using Tyra::Vec4;

namespace Demo {

class Heightmap {
 public:
  Heightmap(const float& minHeight, const float& maxHeight, const Vec4& leftUp,
            const Vec4& rightDown);
  ~Heightmap();

  const float& getHeightOffset(const Vec4& playerPosition) const;

  bool isOutside(const Vec4& position) const;

  float** map;
  short mapWidth, mapHeight;
  float minHeight, maxHeight;
  Vec4 leftUp, rightDown;

 private:
  float getWidthPercentage(const Vec4& playerPosition) const;
  float getHeightPercentage(const Vec4& playerPosition) const;
  void allocateMap(const unsigned char* data, const int& width,
                   const int& height);
  float getGameHeight(const unsigned char& inputColor,
                      const unsigned char& minColor,
                      const unsigned char& maxColor) const;
};

}  // namespace Demo
