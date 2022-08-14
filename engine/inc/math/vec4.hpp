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

extern "C" {
#include <stdio.h>
#include <tamtypes.h>
}

#include <math3d.h>
#include <string>
#include "math/math.hpp"

namespace Tyra {

class Vec4 {
 public:
  union {
    struct {
      float x;
      float y;
      float z;
      float w;
    };
    VECTOR xyzw alignas(sizeof(float) * 4);
  };

  static void copy(Vec4* out, const float* in);
  static inline void copy(Vec4* out, const Vec4& in) { copy(out, in.xyzw); }

  /** Initialize Vec4 without setting default values */
  Vec4() {}

  /** Initialize Vec4 with provided values */
  Vec4(const float& x, const float& y = 0.0F, const float& z = 0.0F,
       const float& w = 1.0F)
      : x(x), y(y), z(z), w(w) {}

  /** Initialize Vec4 with other Vec4 values */
  Vec4(const Vec4& v) { copy(this, v); }

  /** Initialize Vec4 with float[4] values */
  explicit Vec4(const float* v) { copy(this, v); }

  /** copy */
  void operator=(const Vec4& v);

  /** negate xyz */
  Vec4 operator-(void) const;

  /** xyz + xyz */
  Vec4 operator+(const Vec4& v) const;
  void operator+=(const Vec4& v);

  /** xyz - xyz */
  Vec4 operator-(const Vec4& v) const;
  void operator-=(const Vec4& v);

  /** xyzw * xyzw */
  Vec4 operator*(const Vec4& v) const;
  void operator*=(const Vec4& v);

  /** xyzw / xyzw */
  Vec4 operator/(const Vec4& v) const;
  void operator/=(const Vec4& v);

  /** xyz * v */
  Vec4 operator*(const float& v) const;
  void operator*=(const float& v);

  /** xyz / v */
  Vec4 operator/(const float& v) const;
  void operator/=(const float& v);

  /** (0,0,0,1) */
  void unit();

  inline void set(const Vec4& v) { copy(this, v.xyzw); }
  inline void set(const float* v) { copy(this, v); }
  void set(const float& x, const float& y = 0.0F, const float& z = 0.0F,
           const float& w = 1.0F);

  Vec4 cross(const Vec4& v) const;

  void rotateZ(const int& angle);

  int getRelativeCosBetween(const Vec4& v) const;
  int getRelativeAngleBetween(const Vec4& v) const;

  float innerProduct(const Vec4& v) const;
  inline float dot3(const Vec4& v) const { return innerProduct(v); }

  /** Get vector length */
  float length() const;

  /** Normalize vector */
  void normalize();

  Vec4 getNormalized() const;

  /** Returns distance between two vectors */
  float distanceTo(const Vec4& v) const;

  /**
   * Checks intersection with given box
   * @param min Opposite min vertex (ex. near, left, down vertex of bounding
   * box)
   * @param max Opposite max vertex (ex. far, right, up vertex of bounding
   * box)
   */
  bool collidesBox(const Vec4& min, const Vec4& max) const;

  /**
   * Checks if this vector is on given box (this->y >= box.y)
   * @param min Opposite min vertex (ex. near, left, down vertex of bounding
   * box)
   * @param max Opposite max vertex (ex. far, right, up vertex of bounding
   * box)
   */
  bool isOnBox(const Vec4& min, const Vec4& max) const;

  /** Check if given triangle should be backface culled */
  static u8 shouldBeBackfaceCulled(const Vec4* cameraPos, const Vec4* v0,
                                   const Vec4* v1, const Vec4* v2);

  /**
   * Update this vector by linear interpolation between two vertices
   * @param v1 Before vertex
   * @param v2 After vertex
   * @param interp State of interpolation
   */
  void lerp(const Vec4& v1, const Vec4& v2, const float& interp);

  /**
   * Get vector by linear interpolation between two vertices
   * @param v1 Before vertex
   * @param v2 After vertex
   * @param interp State of interpolation
   */
  static Vec4 getByLerp(const Vec4& v1, const Vec4& v2, const float& interp);

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

  static void setLerp(Vec4* output, const Vec4& v1, const Vec4& v2,
                      const float& interp);
};

}  // namespace Tyra
