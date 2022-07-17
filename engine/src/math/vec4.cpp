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

namespace Tyra {

void Vec4::set(const float& t_x, const float& t_y, const float& t_z,
               const float& t_w) {
  x = t_x;
  y = t_y;
  z = t_z;
  w = t_w;
}

Vec4 Vec4::operator+(const Vec4& v) const {
  Vec4 res;
  asm volatile(
      "lqc2       $vf4,   0x0(%1)         \n\t"
      "lqc2       $vf5,   0x0(%2)         \n\t"
      "vadd.xyz   $vf6,   $vf4,   $vf5    \n\t"
      "sqc2       $vf6,   0x0(%0)         \n\t"
      :
      : "r"(res.xyzw), "r"(this->xyzw), "r"(v.xyzw));
  return res;
}

Vec4 Vec4::operator-(const Vec4& v) const {
  Vec4 res;
  asm volatile(
      "lqc2       $vf4,   0x0(%1)         \n\t"
      "lqc2       $vf5,   0x0(%2)         \n\t"
      "vsub.xyz   $vf6,   $vf4,   $vf5    \n\t"
      "sqc2       $vf6,   0x0(%0)         \n\t"
      :
      : "r"(res.xyzw), "r"(this->xyzw), "r"(v.xyzw));
  return res;
}

Vec4 Vec4::operator*(const Vec4& v) const {
  Vec4 res;
  asm volatile(
      "lqc2           $vf4,   0x0(%1)         \n\t"
      "lqc2           $vf5,   0x0(%2)         \n\t"
      "vmul.xyzw      $vf6,   $vf4,   $vf5    \n\t"
      "sqc2           $vf6,   0x0(%0)         \n\t"
      :
      : "r"(res.xyzw), "r"(this->xyzw), "r"(v.xyzw));
  return res;
}

Vec4 Vec4::operator*(const float& v) const {
  Vec4 res;
  asm volatile(
      "lqc2       $vf4,   0x0(%1)         \n\t"
      "mfc1       $8,     %2              \n\t"
      "qmtc2      $8,     $vf5            \n\t"
      "vmulx.xyz  $vf6,   $vf4,   $vf5    \n\t"
      "sqc2       $vf6,   0x0(%0)         \n\t"
      :
      : "r"(res.xyzw), "r"(this->xyzw), "f"(v));
  res.w = w;  // Comment below. TODO: fix it in asm
  return res;
}

Vec4 Vec4::operator/(const float& v) const {
  return Vec4(x / v, y / v, z / v, w);
}

void Vec4::operator+=(const Vec4& v) {
  asm volatile(
      "lqc2      $vf4, 0x0(%0)    \n\t"
      "lqc2      $vf5, 0x0(%1)    \n\t"
      "vadd.xyz  $vf4, $vf4, $vf5 \n\t"
      "sqc2      $vf4, 0x0(%0)    \n\t"
      :
      : "r"(this->xyzw), "r"(v.xyzw));
}

void Vec4::operator*=(const Vec4& v) {
  asm volatile(
      "lqc2           $vf4,   0x0(%0)         \n\t"
      "lqc2           $vf5,   0x0(%1)         \n\t"
      "vmul.xyzw      $vf6,   $vf4,   $vf5    \n\t"
      "sqc2           $vf6,   0x0(%0)         \n\t"
      :
      : "r"(this->xyzw), "r"(v.xyzw));
}

void Vec4::operator*=(const float& v) {
  // Hmm... we don't want to modify W in most of the cases
  // It should be touched only during rendering process
  auto tempW = w;

  asm volatile(
      "lqc2       $vf4, 0x0(%0)  \n\t"
      "mfc1       $8,  %1       \n\t"
      "qmtc2      $8,  $vf5      \n\t"
      "vmulx.xyz  $vf4, $vf4, $vf5 \n\t"
      "sqc2       $vf4, 0x0(%0)  \n\t"
      :
      : "r"(this->xyzw), "f"(v));

  w = tempW;
}

void Vec4::operator/=(const float& v) {
  x /= v;
  y /= v;
  z /= v;
}

void Vec4::operator=(const Vec4& v) { copy(this, v); }

Vec4 Vec4::operator-(void) const { return Vec4(-x, -y, -z); }

void Vec4::copy(Vec4* out, const float* in) {
  asm volatile(
      "lqc2		$vf1, 0x00(%1)	\n"
      "sqc2		$vf1, 0x00(%0)	\n"
      :
      : "r"(out->xyzw), "r"(in));
}

Vec4 Vec4::cross(const Vec4& v) const {
  Vec4 res;
  asm volatile(
      "lqc2           $vf4,   0x0(%1)         \n\t"
      "lqc2           $vf5,   0x0(%2)         \n\t"
      "vopmula.xyz    $ACC,   $vf4,   $vf5    \n\t"
      "vopmsub.xyz    $vf8,   $vf5,   $vf4    \n\t"
      "vsub.w         $vf8,   $vf0,   $vf0    \n\t"
      "sqc2           $vf8,   0x0(%0)         \n\t"
      :
      : "r"(res.xyzw), "r"(this->xyzw), "r"(v.xyzw));
  return res;
}

void Vec4::rotateZ(const int& angle) {
  auto s = Math::sin(angle);
  auto c = Math::cos(angle);
  x = x * c - y * s;
  y = x * s + y * c;
}

int Vec4::getRelativeCosBetween(const Vec4& v) const {
  return dot3(v) / (length() * v.length());
}
int Vec4::getRelativeAngleBetween(const Vec4& v) const {
  return acos(getRelativeCosBetween(v));
}

float Vec4::innerProduct(const Vec4& v) const {
  float result;
  asm volatile(
      "lqc2     $vf4, 0x0(%1)  \n\t"
      "lqc2     $vf5, 0x0(%2)  \n\t"
      "vmul.xyz $vf6, $vf4, $vf5 \n\t"
      "vaddy.x  $vf6, $vf6, $vf6 \n\t"
      "vaddz.x  $vf6, $vf6, $vf6 \n\t"
      "qmfc2    $2,  $vf6      \n\t"
      "mtc1     $2,  %0       \n\t"
      : "=f"(result)
      : "r"(this->xyzw), "r"(v.xyzw));
  return result;
}

float Vec4::length() const {
  float result;
  asm volatile(
      "lqc2     $vf4, 0x0(%1)  \n\t"
      "vmul.xyz $vf5, $vf4, $vf4 \n\t"
      "vaddy.x  $vf5, $vf5, $vf5 \n\t"
      "vaddz.x  $vf5, $vf5, $vf5 \n\t"
      "vsqrt    $Q  , $vf5x     \n\t"
      "vwaitq                 \n\t"
      "vaddq.x  $vf8, $vf0, $Q   \n\t"
      "qmfc2    $2,  $vf8      \n\t"
      "mtc1     $2,  %0       \n\t"
      : "=f"(result)
      : "r"(this->xyzw));
  return result;
}

void Vec4::normalize() {
  asm volatile(
      "lqc2       $vf4, 0x0(%0)    \n\t"
      "vmul.xyz   $vf5, $vf4,  $vf4  \n\t"
      "vaddy.x    $vf5, $vf5,  $vf5  \n\t"
      "vaddz.x    $vf5, $vf5,  $vf5  \n\t"
      "vrsqrt     $Q,   $vf0w, $vf5x \n\t"
      "vwaitq                     \n\t"
      "vsub.xyz   $vf6, $vf0,  $vf0  \n\t"
      "vaddw.xyz  $vf6, $vf6,  $vf4  \n\t"
      "vwaitq                     \n\t"
      "vmulq.xyz  $vf6, $vf4,  $Q    \n\t"
      "sqc2       $vf6, 0x0(%0)    \n\t"
      :
      : "r"(this->xyzw));
}

float Vec4::distanceTo(const Vec4& v) const {
  float result;
  asm volatile(
      "lqc2     $vf4, 0x0(%1)  \n\t"
      "lqc2     $vf5, 0x0(%2)  \n\t"
      "vsub.xyz $vf6, $vf4, $vf5 \n\t"
      "vmul.xyz $vf7, $vf6, $vf6 \n\t"
      "vaddy.x  $vf7, $vf7, $vf7 \n\t"
      "vaddz.x  $vf7, $vf7, $vf7 \n\t"
      "vsqrt    $Q  , $vf7x     \n\t"
      "vwaitq                 \n\t"
      "vaddq.x  $vf8, $vf0, $Q   \n\t"
      "qmfc2    $2,  $vf8      \n\t"
      "mtc1     $2,  %0       \n\t"
      : "=f"(result)
      : "r"(this->xyzw), "r"(v.xyzw));
  return result;
}

u8 Vec4::shouldBeBackfaceCulled(const Vec4* cameraPos, const Vec4* v0,
                                const Vec4* v1, const Vec4* v2) {
  float dot;
  asm volatile(
      "lqc2        $vf4, 0x0(%1)    \n\t"  // $vf4 = cameraPos
      "lqc2        $vf5, 0x0(%2)    \n\t"  // $vf5 = v0
      "lqc2        $vf6, 0x0(%3)    \n\t"  // $vf6 = v1
      "lqc2        $vf7, 0x0(%4)    \n\t"  // $vf7 = v2
      "vsub.xyz    $vf8, $vf7, $vf5 \n\t"  // $vf8 = $vf7(v2) - $vf5(v0)
      "vsub.xyz    $vf9, $vf6, $vf5 \n\t"  // $vf9 = $vf6(v1) - $vf5(v0)
      "vopmula.xyz $ACC, $vf8, $vf9 \n\t"  // $vf6 = cross($vf8, $vf9)
      "vopmsub.xyz $vf6, $vf9, $vf8 \n\t"
      "vsub.w      $vf6, $vf6, $vf6 \n\t"
      "vsub.xyz    $vf7, $vf5, $vf4 \n\t"  // $vf7 = $vf5(v0) - $vf4(cameraPos)
      "vmul.xyz    $vf5, $vf7, $vf6 \n\t"  // $vf5 = dot($vf7, $vf6)
      "vaddy.x     $vf5, $vf5, $vf5 \n\t"
      "vaddz.x     $vf5, $vf5, $vf5 \n\t"
      "qmfc2       $2,  $vf5        \n\t"  // store result on `dot` variable
      "mtc1        $2,  %0          \n\t"
      : "=f"(dot)
      : "r"(cameraPos->xyzw), "r"(v0->xyzw), "r"(v1->xyzw), "r"(v2->xyzw));
  return dot <= 0.0F;
}

void Vec4::lerp(const Vec4& v1, const Vec4& v2, const float& interp) {
  setLerp(this, v1, v2, interp);
}

Vec4 Vec4::getByLerp(const Vec4& v1, const Vec4& v2, const float& interp) {
  Vec4 result;
  setLerp(&result, v1, v2, interp);
  return result;
}

void Vec4::setLerp(Vec4* output, const Vec4& v1, const Vec4& v2,
                   const float& interp) {
  asm volatile(
      "lqc2       $vf4, 0x0(%1)       \n\t"  // $vf4 = v1
      "lqc2       $vf5, 0x0(%2)       \n\t"  // $vf5 = v2
      "mfc1       $8,  %3             \n\t"  // $vf6 = t
      "qmtc2      $8,  $vf6           \n\t"  // lerp:
      "vsub.xyzw  $vf7, $vf5, $vf4    \n\t"  // $vf7 = v2 - v1
      "vmulx.xyzw $vf8, $vf7, $vf6    \n\t"  // $vf8 = $vf7 * t
      "vadd.xyzw  $vf9, $vf8, $vf4    \n\t"  // $vf9 = $vf8 + $vf4
      "sqc2       $vf9, 0x0(%0)       \n\t"  // v0  = $vf9
      :
      : "r"(&output->xyzw), "r"(&v1.xyzw), "r"(&v2.xyzw), "f"(interp));
}

void Vec4::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void Vec4::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string Vec4::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "Vec4(";
  }
  res << std::fixed << std::setprecision(4);
  res << x << ", " << y << ", " << z << ", " << w << ")";
  return res.str();
}

}  // namespace Tyra
