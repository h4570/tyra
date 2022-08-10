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

#include "math/vec4.hpp"
#include "math/math.hpp"
#include <string>

namespace Tyra {

class M4x4 {
 public:
  MATRIX data alignas(sizeof(float) * 16);

  static const M4x4 Identity;

  /** Create matrix with random values */
  M4x4() {}

  M4x4(const M4x4& v) { copy(this, v); }

  explicit M4x4(const float* v) { copy(this, v); }

  M4x4(const float& m11, const float& m12, const float& m13, const float& m14,
       const float& m21, const float& m22, const float& m23, const float& m24,
       const float& m31, const float& m32, const float& m33, const float& m34,
       const float& m41, const float& m42, const float& m43, const float& m44);

  static void copy(M4x4* out, const float* in);
  static void copy(M4x4* out, const M4x4& in);

  void operator=(const M4x4& v);
  Vec4 operator*(const Vec4& v) const;
  M4x4 operator*(const M4x4& v) const;
  void operator*=(const M4x4& v);
  float& operator[](const u8& index);

  /**
   * 1 0 0 0
   * 0 1 0 0
   * 0 0 1 0
   * 0 0 0 1
   */
  void identity();

  inline void unit() { identity(); }

  void set(const float& m11, const float& m12, const float& m13,
           const float& m14, const float& m21, const float& m22,
           const float& m23, const float& m24, const float& m31,
           const float& m32, const float& m33, const float& m34,
           const float& m41, const float& m42, const float& m43,
           const float& m44);

  void translate(const Vec4& v);
  void translateX(const float& v);
  void translateY(const float& v);
  void translateZ(const float& v);

  void rotateByAngle(const float& angle, const Vec4& axis);
  void rotate(const Vec4& v);
  void rotateX(const float& radians);
  void rotateY(const float& radians);
  void rotateZ(const float& radians);

  void scale(const float& v);
  void scale(const Vec4& v);
  void scaleX(const float& v);
  void scaleY(const float& v);
  void scaleZ(const float& v);

  /** Create perspective projection M4x4 (gluPerspective) */
  static M4x4 perspective(const float& fov, const float& width,
                          const float& height, const float& projectionScale,
                          const float& aspectRatio, const float& near,
                          const float& far);

  /**
   * Create a view M4x4 that transforms coordinates in
   * such a way that the user looks at a target vector
   * direction from a position vector (glLookAt)
   */
  static M4x4 lookAt(const Vec4& position, const Vec4& target);
  static void lookAt(M4x4* res, const Vec4& position, const Vec4& target);

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 private:
  static float upVec[4] alignas(sizeof(float) * 4);
  static float viewVec[4] alignas(sizeof(float) * 4);

  static void cross(float res[16], const float a[16], const float b[16]);
  void rotationX(const float& v);
  void rotationY(const float& v);
  void rotationZ(const float& v);
  void rotationByAngle(const float& angle, const Vec4& axis);
  void translationX(const float& v);
  void translationY(const float& v);
  void translationZ(const float& v);
  void setScale(const Vec4& val);
  static M4x4 setCamera(const float pos[4], const float vz[4],
                        const float vy[4]);
};

}  // namespace Tyra
