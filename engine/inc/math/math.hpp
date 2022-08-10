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

extern "C" {
#include <tamtypes.h>
}

#include <math.h>

namespace Tyra {

class Math {
 public:
  constexpr static float HALF_ANG2RAD = 3.14159265358979323846F / 360.0F;
  constexpr static float ANG2RAD = 3.14159265358979323846F / 180.0F;
  constexpr static float PI = 3.1415926535897932384626433832795F;
  constexpr static float HALF_PI = 1.5707963267948966192313216916398F;
  static float cos(float x);
  static float asin(float x);
  static float atan2(float y, float x);
  static float mod(float x, float y);
  static float acos(float x);
  static float sin(float x);
  static float tan(float x);
  static float invSqrt(float x);
  static bool equalf(const float& a, const float& b,
                     const float& epsilon = 0.00001F);

 private:
  Math();
};

}  // namespace Tyra
