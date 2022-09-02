/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include "math/vec4.hpp"
#include <string>

#pragma once

namespace Tyra {

/**
 * Class for raycasting.
 */
class Ray {
 public:
  /** @brief Constructor create a ampty ray */
  Ray();

  /**
   * @param origin The origin of the ray
   * @param direction The direction of the ray (normalized Vec4)
   */
  Ray(const Vec4& origin, const Vec4& direction);
  ~Ray();

  Vec4 origin;
  Vec4 direction;

  /**
   * @return Vec4 that is a given distance along this Ray
   * @param t - the distance along the Ray to retrieve a position for.
   */
  Vec4 at(const float& t) const;

  /** @return Distance from the Vec4 point to the origin */
  float distanceToPoint(const Vec4& point) const;

  /**
   * @param minCorner - pointer to min corner position. (bottom left)
   * @param maxCorner - pointer to max corner position. (top right)
   * @param outputDistance - optional - distance the box to intersect with.
   * @return Vec4 point of intersection
   */
  bool intersectBox(const Vec4& minCorner, const Vec4& maxCorner,
                    float* outputDistance = nullptr) const;

  /** Returns inverse direction */
  Vec4 invDir() const;

  void print() const;
  void print(const char* name) const;
  void print(const std::string& name) const { print(name.c_str()); }
  std::string getPrint(const char* name = nullptr) const;
};

}  // namespace Tyra