/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "shared/math/vec2.hpp"
#include <iomanip>

namespace Tyra {

Vec2::Vec2(const float& t_x, const float& t_y) {
  x = t_x;
  y = t_y;
}

Vec2::Vec2(const Vec2& v) {
  x = v.x;
  y = v.y;
}

Vec2::Vec2() {
  x = 0;
  y = 0;
}

Vec2::~Vec2() {}

void Vec2::set(const float& t_x, const float& t_y) {
  x = t_x;
  y = t_y;
}

void Vec2::set(const Vec2& v) {
  x = v.x;
  y = v.y;
}

void Vec2::rotate(const float& t_angle, const float& t_x, const float& t_y) {
  float s = Math::sin(t_angle);
  float c = Math::cos(t_angle);

  x -= t_x;
  y -= t_y;

  float xnew = x * c - y * s;
  float ynew = x * s + y * c;

  x = xnew + t_x;
  y = ynew + t_y;
}

void Vec2::operator=(const Vec2& v) {
  x = v.x;
  y = v.y;
}

Vec2 Vec2::operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
Vec2 Vec2::operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
Vec2 Vec2::operator*(const Vec2& v) const { return Vec2(x * v.x, y * v.y); }
Vec2 Vec2::operator/(const Vec2& v) const { return Vec2(x / v.x, y / v.y); }

Vec2 Vec2::operator+(const float& v) const { return Vec2(x + v, y + v); }
Vec2 Vec2::operator-(const float& v) const { return Vec2(x - v, y - v); }
Vec2 Vec2::operator*(const float& v) const { return Vec2(x * v, y * v); }
Vec2 Vec2::operator/(const float& v) const { return Vec2(x / v, y / v); }

void Vec2::operator+=(const Vec2& v) {
  x += v.x;
  y += v.y;
}
void Vec2::operator-=(const Vec2& v) {
  x -= v.x;
  y -= v.y;
}
void Vec2::operator*=(const Vec2& v) {
  x *= v.x;
  y *= v.y;
}
void Vec2::operator/=(const Vec2& v) {
  x /= v.x;
  y /= v.y;
}

void Vec2::operator+=(const float& v) {
  x += v;
  y += v;
}
void Vec2::operator-=(const float& v) {
  x -= v;
  y -= v;
}
void Vec2::operator*=(const float& v) {
  x *= v;
  y *= v;
}
void Vec2::operator/=(const float& v) {
  x /= v;
  y /= v;
}

void Vec2::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void Vec2::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string Vec2::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "Vec2(";
  }
  res << std::fixed << std::setprecision(4);
  res << x << ", " << y << ")";
  return res.str();
}

}  // namespace Tyra
