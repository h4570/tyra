/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <stdio.h>
#include <iomanip>
#include <sstream>
#include "math/m4x4.hpp"

namespace Tyra {

VECTOR M4x4::upVec = {0.0F, 1.0F, 0.0F, 1.0F};
VECTOR M4x4::viewVec = {0.0F, 0.0F, 0.0F, 1.0F};

const M4x4 M4x4::Identity =
    M4x4(1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F,
         0.0F, 0.0F, 0.0F, 1.0F);

M4x4::M4x4(const float& m11, const float& m12, const float& m13,
           const float& m14, const float& m21, const float& m22,
           const float& m23, const float& m24, const float& m31,
           const float& m32, const float& m33, const float& m34,
           const float& m41, const float& m42, const float& m43,
           const float& m44) {
  set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43,
      m44);
}

void M4x4::copy(M4x4* out, const float* in) {
  asm volatile(
      "lqc2		$vf1, 0x00(%1)  \n"
      "lqc2		$vf2, 0x10(%1)  \n"
      "lqc2		$vf3, 0x20(%1)  \n"
      "lqc2		$vf4, 0x30(%1)  \n"
      "sqc2		$vf1, 0x00(%0)  \n"
      "sqc2		$vf2, 0x10(%0)  \n"
      "sqc2		$vf3, 0x20(%0)  \n"
      "sqc2		$vf4, 0x30(%0)  \n"
      :
      : "r"(out->data), "r"(in));
}

void M4x4::copy(M4x4* out, const M4x4& in) { copy(out, in.data); }

void M4x4::operator=(const M4x4& v) { copy(this, v); }

Vec4 M4x4::operator*(const Vec4& v) const {
  Vec4 result;
  asm volatile(
      "lqc2		$vf1, 0x00(%2)	\n"
      "lqc2		$vf2, 0x10(%2)	\n"
      "lqc2		$vf3, 0x20(%2)	\n"
      "lqc2		$vf4, 0x30(%2)	\n"
      "lqc2		$vf5, 0x00(%1)	\n"
      "vmulaw	$ACC, $vf4, $vf0\n"
      "vmaddax	$ACC, $vf1, $vf5\n"
      "vmadday	$ACC, $vf2, $vf5\n"
      "vmaddz	$vf6, $vf3, $vf5\n"
      "sqc2		$vf6, 0x00(%0)	\n"
      :
      : "r"(result.xyzw), "r"(v.xyzw), "r"(this->data));
  return result;
}

M4x4 M4x4::operator*(const M4x4& v) const {
  M4x4 result;
  cross(result.data, this->data, v.data);
  return result;
}

void M4x4::operator*=(const M4x4& v) { cross(this->data, this->data, v.data); }

float& M4x4::operator[](const u8& index) { return data[index]; }

void M4x4::set(const float& m11, const float& m12, const float& m13,
               const float& m14, const float& m21, const float& m22,
               const float& m23, const float& m24, const float& m31,
               const float& m32, const float& m33, const float& m34,
               const float& m41, const float& m42, const float& m43,
               const float& m44) {
  data[0] = m11;
  data[1] = m12;
  data[2] = m13;
  data[3] = m14;

  data[4] = m21;
  data[5] = m22;
  data[6] = m23;
  data[7] = m24;

  data[8] = m31;
  data[9] = m32;
  data[10] = m33;
  data[11] = m34;

  data[12] = m41;
  data[13] = m42;
  data[14] = m43;
  data[15] = m44;
}

void M4x4::identity() {
  asm volatile(
      "vsub.xyzw  $vf4, $vf0, $vf0 \n\t"
      "vadd.w     $vf4, $vf4, $vf0 \n\t"
      "vmr32.xyzw $vf5, $vf4      \n\t"
      "vmr32.xyzw $vf6, $vf5      \n\t"
      "vmr32.xyzw $vf7, $vf6      \n\t"
      "sqc2       $vf4, 0x30(%0) \n\t"
      "sqc2       $vf5, 0x20(%0) \n\t"
      "sqc2       $vf6, 0x10(%0) \n\t"
      "sqc2       $vf7, 0x0(%0)  \n\t"
      :
      : "r"(this->data));
}

void M4x4::translate(const Vec4& v) {
  M4x4 temp = M4x4::Identity;
  temp.translationX(v.x);
  temp.translationY(v.y);
  temp.translationZ(v.z);
  cross(this->data, temp.data, this->data);
}

void M4x4::translateX(const float& v) {
  M4x4 temp = M4x4::Identity;
  temp.translationX(v);
  cross(this->data, temp.data, this->data);
}

void M4x4::translateY(const float& v) {
  M4x4 temp = M4x4::Identity;
  temp.translationY(v);
  cross(this->data, temp.data, this->data);
}

void M4x4::translateZ(const float& v) {
  M4x4 temp = M4x4::Identity;
  temp.translationZ(v);
  cross(this->data, temp.data, this->data);
}

void M4x4::rotate(const Vec4& v) {
  M4x4 temp = M4x4::Identity;

  temp.rotationZ(v.z);
  cross(this->data, temp.data, this->data);

  temp.identity();
  temp.rotationY(v.y);
  cross(this->data, temp.data, this->data);

  temp.identity();
  temp.rotationX(v.x);
  cross(this->data, temp.data, this->data);
}

void M4x4::rotateX(const float& radians) {
  M4x4 temp = M4x4::Identity;
  temp.rotationX(radians);
  cross(this->data, temp.data, this->data);
}

void M4x4::rotateY(const float& radians) {
  M4x4 temp = M4x4::Identity;
  temp.rotationY(radians);
  cross(this->data, temp.data, this->data);
}

void M4x4::rotateZ(const float& radians) {
  M4x4 temp = M4x4::Identity;
  temp.rotationZ(radians);
  cross(this->data, temp.data, this->data);
}

void M4x4::rotateByAngle(const float& angle, const Vec4& axis) {
  M4x4 temp;
  temp.rotationByAngle(angle, axis);
  cross(this->data, temp.data, this->data);
}

void M4x4::scale(const float& v) { scale(Vec4(v, v, v, 1.0F)); }

void M4x4::scale(const Vec4& v) {
  M4x4 temp = M4x4::Identity;
  temp.setScale(v);
  cross(this->data, temp.data, this->data);
}

void M4x4::scaleX(const float& v) { scale(Vec4(v, 1.0F, 1.0F, 1.0F)); }

void M4x4::scaleY(const float& v) { scale(Vec4(1.0F, v, 1.0F, 1.0F)); }

void M4x4::scaleZ(const float& v) { scale(Vec4(1.0F, 1.0F, v, 1.0F)); }

M4x4 M4x4::perspective(const float& fov, const float& width,
                       const float& height, const float& projectionScale,
                       const float& aspectRatio, const float& near,
                       const float& far) {
  M4x4 res;
  float fovYdiv2 = Math::HALF_ANG2RAD * fov;
  float cotFOV = 1.0F / (Math::sin(fovYdiv2) / Math::cos(fovYdiv2));
  float w = cotFOV * (width / projectionScale) / aspectRatio;
  float h = cotFOV * (height / projectionScale);

  res.data[0] = w;
  res.data[1] = 0.0F;
  res.data[2] = 0.0F;
  res.data[3] = 0.0F;

  res.data[4] = 0.0F;
  res.data[5] = -h;
  res.data[6] = 0.0F;
  res.data[7] = 0.0F;

  res.data[8] = 0.0F;
  res.data[9] = 0.0F;
  res.data[10] = (far + near) / (far - near);
  res.data[11] = -1.0F;

  res.data[12] = 0.0F;
  res.data[13] = 0.0F;
  res.data[14] = (2.0F * far * near) / (far - near);
  res.data[15] = 0.0F;
  return res;
}

M4x4 M4x4::lookAt(const Vec4& position, const Vec4& target) {
  M4x4 res = M4x4::Identity;
  lookAt(&res, position, target);
  return res;
}

void M4x4::lookAt(M4x4* res, const Vec4& position, const Vec4& target) {
  float eye[4] alignas(sizeof(float) * 4) = {position.x, position.y, position.z,
                                             1.0F};

  float obj[4] alignas(sizeof(float) * 4) = {target.x, target.y, target.z,
                                             1.0F};

  asm volatile(
      // eye
      "lqc2          $vf4, 0x00(%2)      \n\t"
      // obj
      "lqc2		    $vf5, 0x00(%3)      \n\t"
      // view_vec = $vf7
      "vsub.xyz	    $vf7, $vf4, $vf5    \n\t"
      "vmove.xyzw   $vf6, $vf0          \n\t"
      // $vf6 = { 0.0f, 1.0f, 0.0f, 1.0f }
      "vaddw.y		$vf6, $vf0, $vf0    \n\t"
      "vopmula.xyz	$ACC, $vf6, $vf7    \n\t"
      // vec = $vf9
      "vopmsub.xyz	$vf9, $vf7, $vf6    \n\t"
      "vopmula.xyz	$ACC, $vf7, $vf9    \n\t"
      // up_vec = $vf8
      "vopmsub.xyz	$vf8, $vf9, $vf7    \n\t"
      // view_vec
      "sqc2		    $vf7, 0x00(%0)      \n\t"
      // up_vec
      "sqc2		    $vf6, 0x00(%1)      \n\t"
      :
      : "r"(viewVec), "r"(upVec), "r"(eye), "r"(obj));

  M4x4 temp = setCamera(eye, viewVec, upVec);
  res->identity();

  cross(res->data, res->data, temp.data);
}

void M4x4::cross(float res[16], const float a[16], const float b[16]) {
  asm volatile(
      "lqc2         $vf1, 0x00(%1) \n\t"
      "lqc2         $vf2, 0x10(%1) \n\t"
      "lqc2         $vf3, 0x20(%1) \n\t"
      "lqc2         $vf4, 0x30(%1) \n\t"
      "lqc2         $vf5, 0x00(%2) \n\t"
      "lqc2         $vf6, 0x10(%2) \n\t"
      "lqc2         $vf7, 0x20(%2) \n\t"
      "lqc2         $vf8, 0x30(%2) \n\t"
      "vmulax.xyzw  $ACC, $vf5, $vf1 \n\t"
      "vmadday.xyzw $ACC, $vf6, $vf1 \n\t"
      "vmaddaz.xyzw $ACC, $vf7, $vf1 \n\t"
      "vmaddw.xyzw  $vf1, $vf8, $vf1 \n\t"
      "vmulax.xyzw  $ACC, $vf5, $vf2 \n\t"
      "vmadday.xyzw $ACC, $vf6, $vf2 \n\t"
      "vmaddaz.xyzw $ACC, $vf7, $vf2 \n\t"
      "vmaddw.xyzw  $vf2, $vf8, $vf2 \n\t"
      "vmulax.xyzw  $ACC, $vf5, $vf3 \n\t"
      "vmadday.xyzw $ACC, $vf6, $vf3 \n\t"
      "vmaddaz.xyzw $ACC, $vf7, $vf3 \n\t"
      "vmaddw.xyzw  $vf3, $vf8, $vf3 \n\t"
      "vmulax.xyzw  $ACC, $vf5, $vf4 \n\t"
      "vmadday.xyzw $ACC, $vf6, $vf4 \n\t"
      "vmaddaz.xyzw $ACC, $vf7, $vf4 \n\t"
      "vmaddw.xyzw  $vf4, $vf8, $vf4 \n\t"
      "sqc2         $vf1, 0x00(%0) \n\t"
      "sqc2         $vf2, 0x10(%0) \n\t"
      "sqc2         $vf3, 0x20(%0) \n\t"
      "sqc2         $vf4, 0x30(%0) \n\t"
      :
      : "r"(res), "r"(b), "r"(a)
      : "memory");
}

void M4x4::rotationX(const float& v) {
  float c = Math::cos(v);
  float s = Math::sin(v);
  this->data[5] = c;   // 1,1
  this->data[6] = s;   // 1,2
  this->data[9] = -s;  // 2,1
  this->data[10] = c;  // 2,2
}

void M4x4::rotationY(const float& v) {
  float c = Math::cos(v);
  float s = Math::sin(v);
  this->data[0] = c;   // 0,0
  this->data[2] = -s;  // 0,3
  this->data[8] = s;   // 2,0
  this->data[10] = c;  // 2,2
}

void M4x4::rotationZ(const float& v) {
  float c = Math::cos(v);
  float s = Math::sin(v);
  this->data[0] = c;   // 0,0
  this->data[1] = s;   // 0,1
  this->data[4] = -s;  // 1,0
  this->data[5] = c;   // 1,1
}

void M4x4::rotationByAngle(const float& angle, const Vec4& axis) {
  Vec4 localAxis = Vec4(axis);
  localAxis.normalize();
  const float& x = localAxis.x;
  const float& y = localAxis.y;
  const float& z = localAxis.z;

  float c = Math::cos(angle);
  float s = Math::sin(angle);

  this->data[0] = x * x * (1 - c) + c;
  this->data[1] = y * x * (1 - c) + z * s;
  this->data[2] = x * z * (1 - c) - y * s;
  this->data[3] = 0.0F;

  this->data[4] = x * y * (1 - c) - z * s;
  this->data[5] = y * y * (1 - c) + c;
  this->data[6] = y * z * (1 - c) + x * s;
  this->data[7] = 0.0F;

  this->data[8] = x * z * (1 - c) + y * s;
  this->data[9] = y * z * (1 - c) - x * s;
  this->data[10] = z * z * (1 - c) + c;
  this->data[11] = 0.0F;

  this->data[12] = 0.0F;
  this->data[13] = 0.0F;
  this->data[14] = 0.0F;
  this->data[15] = 1.0F;
}

void M4x4::translationX(const float& val) {
  this->data[12] = val;  // 3,0
}

void M4x4::translationY(const float& val) {
  this->data[13] = val;  // 3,1
}

void M4x4::translationZ(const float& val) {
  this->data[14] = val;  // 3,2
}

void M4x4::setScale(const Vec4& val) {
  this->data[0] = val.x;
  this->data[5] = val.y;
  this->data[10] = val.z;
  this->data[15] = 1.0F;
}

M4x4 M4x4::setCamera(const float pos[4], const float vz[4], const float vy[4]) {
  M4x4 res;
  // M4x4  $vf4, $vf5, $vf6, $vf7
  // pos   $vf8
  // vz    $vf9
  // vy    $vf10
  // vtmp    $vf11
  asm volatile(
      "lqc2         $vf9, 0x00(%2)          \n\t"
      // mtmp.unit()
      "lqc2		    $vf10, 0x00(%3)         \n\t"
      // mtmp[1][PW] = 0.0F
      "vsub.w       $vf5, $vf0, $vf0        \n\t"
      // vtmp.outerProduct(vy, vz);
      "vopmula.xyz  $ACC, $vf10, $vf9       \n\t"
      "vopmsub.xyz	$vf11, $vf9, $vf10      \n\t"
      // mtmp[0] = vtmp.normalize();
      "vmul.xyz	    $vf12, $vf11, $vf11     \n\t"
      "vaddy.x		$vf12, $vf12, $vf12     \n\t"
      "vaddz.x		$vf12, $vf12, $vf12     \n\t"
      "vrsqrt       $Q, $vf0w, $vf12x       \n\t"
      "vsub.xyzw    $vf4, $vf0, $vf0        \n\t"
      "vwaitq                               \n\t"
      "vmulq.xyz    $vf4, $vf11, $Q         \n\t"
      // mtmp[2] = vz.normalize();
      "vmul.xyz     $vf12, $vf9, $vf9       \n\t"
      "vaddy.x		$vf12, $vf12, $vf12     \n\t"
      "vaddz.x		$vf12, $vf12, $vf12     \n\t"
      "vrsqrt       $Q, $vf0w, $vf12x       \n\t"
      "vsub.xyzw    $vf6, $vf0, $vf0        \n\t"
      "vwaitq                               \n\t"
      "vmulq.xyz    $vf6, $vf9, $Q          \n\t"
      // mtmp[1].outerProduct(mtmp[2], mtmp[0]);
      "vopmula.xyz	$ACC, $vf6, $vf4        \n\t"
      "vopmsub.xyz	$vf5, $vf4, $vf6        \n\t"
      // mtmp.transpose(pos);
      "lqc2		    $vf7, 0x00(%1)          \n\t"
      // m = mtmp.inverse();
      "qmfc2.ni		$11, $vf0               \n\t"
      "qmfc2.ni		$8, $vf4                \n\t"
      "qmfc2.ni		$9, $vf5                \n\t"
      "qmfc2.ni		$10, $vf6               \n\t"

      "pextlw		$12, $9, $8             \n\t"
      "pextuw		$13, $9, $8             \n\t"
      "pextlw		$14, $11, $10           \n\t"
      "pextuw		$15, $11, $10           \n\t"
      "pcpyld		$8, $14, $12            \n\t"
      "pcpyud		$9, $12, $14            \n\t"
      "pcpyld		$10, $15, $13           \n\t"

      "qmtc2.ni		$8, $vf16               \n\t"
      "qmtc2.ni		$9, $vf17               \n\t"
      "qmtc2.ni		$10, $vf18              \n\t"
      "vmulax.xyz	$ACC, $vf16, $vf7       \n\t"
      "vmadday.xyz	$ACC, $vf17, $vf7       \n\t"
      "vmaddz.xyz	$vf5, $vf18, $vf7       \n\t"
      "vsub.xyzw	$vf5, $vf0, $vf5        \n\t"

      "sq			$8, 0x00(%0)            \n\t"
      "sq			$9, 0x10(%0)            \n\t"
      "sq			$10, 0x20(%0)           \n\t"
      "sqc2			$vf5, 0x30(%0)          \n\t"
      :
      : "r"(res.data), "r"(pos), "r"(vz), "r"(vy));
  return res;
}

void M4x4::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void M4x4::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string M4x4::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "M4x4(";
  }
  res << std::fixed << std::setprecision(2);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      auto index = (i * 4) + j;
      res << data[index];
      if (index != 15) {
        res << ", ";
      }
    }

    if (i != 3) {
      res << std::endl;
    }
  }
  res << ")";
  return res.str();
}

}  // namespace Tyra
