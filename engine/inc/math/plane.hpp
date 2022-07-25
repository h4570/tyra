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

#include "./vec4.hpp"
#include <string>

namespace Tyra {

class Plane {
 public:
  Vec4 normal;
  float distance;

  Plane();
  Plane(const Vec4& a, const Vec4& b, const Vec4& c);
  ~Plane();

  void update(const Vec4& a, const Vec4& b, const Vec4& c);
  inline float distanceTo(const Vec4& t_vec) const {
    return this->distance + this->normal.innerProduct(t_vec);
  }

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;
};

}  // namespace Tyra
