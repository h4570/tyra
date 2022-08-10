/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "math/vec4.hpp"
#include <iomanip>
#include <sstream>
#include "math/plane.hpp"

namespace Tyra {

Plane::Plane() { this->distance = 0; }

Plane::Plane(const Vec4& a, const Vec4& b, const Vec4& c) {
  this->update(a, b, c);
}

Plane::~Plane() {}

float Plane::distanceTo(const Vec4& t_vec) const {
  return this->distance + this->normal.innerProduct(t_vec);
}

void Plane::update(const Vec4& a, const Vec4& b, const Vec4& c) {
  Vec4 aux1 = a - b;
  Vec4 aux2 = c - b;
  this->normal = aux2.cross(aux1);
  this->normal.normalize();
  this->distance = -this->normal.innerProduct(b);
}

void Plane::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void Plane::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string Plane::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "Plane(";
  }

  res << std::fixed << std::setprecision(4);
  res << "distance: " << distance << ", " << normal.getPrint("normal") << ")";
  return res.str();
}

}  // namespace Tyra
