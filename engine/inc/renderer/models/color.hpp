/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020 - 2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <string>
#include "math/vec4.hpp"

namespace Tyra {

class Color {
 public:
  union {
    struct {
      /** 0-255 */
      float r;
      /** 0-255 */
      float g;
      /** 0-255 */
      float b;
      /** 0-128 */
      float a;
    };
    VECTOR rgba alignas(sizeof(float) * 4);
  };

  static void copy(Color* out, const float* in);
  static inline void copy(Color* out, const Color& in) { copy(out, in.rgba); }

  /** Initialize Color without setting default values */
  Color() {}

  /** Initialize Color with provided values */
  Color(const float& r, const float& g = 255.0F, const float& b = 255.0F,
        const float& a = 128.0F)
      : r(r), g(g), b(b), a(a) {}

  /** Initialize Color with other Color values */
  Color(const Color& v) { copy(this, v); }

  /** Initialize Color with float[4] values */
  explicit Color(const float* v) { copy(this, v); }

  void operator=(const Color& v);

  inline void set(const Color& v) { copy(this, v.rgba); }
  inline void set(const float* v) { copy(this, v); }
  void set(const float& r, const float& g = 255.0F, const float& b = 255.0F,
           const float& a = 128.0F);

  /**
   * Update this vector by linear interpolation between two vertices
   * @param v1 Before vertex
   * @param v2 After vertex
   * @param interp State of interpolation
   */
  void lerp(const Color& v1, const Color& v2, const float& interp);

  /**
   * Get vector by linear interpolation between two vertices
   * @param v1 Before vertex
   * @param v2 After vertex
   * @param interp State of interpolation
   */
  static Color getByLerp(const Color& v1, const Color& v2, const float& interp);

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;

 private:
  static void setLerp(Color* output, const Color& v1, const Color& v2,
                      const float& interp);
};

}  // namespace Tyra
