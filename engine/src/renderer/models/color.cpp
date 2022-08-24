/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/models/color.hpp"
#include <sstream>
#include <iomanip>

namespace Tyra {

void Color::copy(Color* out, const float* in) {
  Vec4::copy(reinterpret_cast<Vec4*>(out), in);
}

void Color::operator=(const Color& v) { copy(this, v); }

void Color::operator+=(const float& v) {
  r += v;
  g += v;
  b += v;
  a += v;
}
void Color::operator-=(const float& v) {
  r -= v;
  g -= v;
  b -= v;
  a -= v;
}

void Color::operator*=(const float& v) {
  r *= v;
  g *= v;
  b *= v;
  a *= v;
}

void Color::operator/=(const float& v) {
  r /= v;
  g /= v;
  b /= v;
  a /= v;
}

Color Color::operator+(const float& v) const {
  Color result = *this;
  result += v;
  return result;
}

Color Color::operator-(const float& v) const {
  Color result = *this;
  result -= v;
  return result;
}

Color Color::operator*(const float& v) const {
  Color result = *this;
  result *= v;
  return result;
}

Color Color::operator/(const float& v) const {
  Color result = *this;
  result /= v;
  return result;
}

void Color::set(const float& t_r, const float& t_g, const float& t_b,
                const float& t_a) {
  r = t_r;
  g = t_g;
  b = t_b;
  a = t_a;
}

void Color::lerp(const Color& v1, const Color& v2, const float& interp) {
  setLerp(this, v1, v2, interp);
}

Color Color::getByLerp(const Color& v1, const Color& v2, const float& interp) {
  Color result;
  setLerp(&result, v1, v2, interp);
  return result;
}

void Color::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void Color::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string Color::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "Color(";
  }
  res << std::fixed << std::setprecision(4);
  res << r << ", " << g << ", " << b << ", " << a << ")";
  return res.str();
}

void Color::setLerp(Color* output, const Color& v1, const Color& v2,
                    const float& interp) {
  Vec4::setLerp(reinterpret_cast<Vec4*>(output),
                reinterpret_cast<const Vec4&>(v1),
                reinterpret_cast<const Vec4&>(v2), interp);
}

}  // namespace Tyra
