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
#include <stdio.h>
#include <tamtypes.h>
}

#include <string>
#include "math/math.hpp"

namespace Tyra {

class Vec2 {
 public:
  union {
    struct {
      float x;
      float y;
    };
    float xy[2] alignas(sizeof(float) * 2);
  };

  Vec2(const float& t_x, const float& t_y);
  Vec2(const Vec2& v);
  Vec2();
  ~Vec2();

  void set(const float& t_x, const float& t_y);
  void set(const Vec2& v);
  void rotate(const float& t_angle, const float& t_x, const float& t_y);

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;
};

}  // namespace Tyra
