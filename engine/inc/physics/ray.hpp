/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022-2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#include "math/vec4.hpp"

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

  // Methods

  /**
   * @param origin Vec4 starting point of the ray;
   * @param direction Vec4 normalized vector pointing to direction;
   */
  void set(const Vec4& origin, const Vec4& direction);

  /**
   * @brief Set origin of the ray;
   * @param origin Vec4 starting point of the ray;
   */
  void setOrigin(const Vec4& origin);

  /**
   * @brief Set direction of the ray;
   * @param direction Vec4 normalized vector pointing to direction;
   */
  void setDirection(const Vec4& direction);

  /**
   * @return Vec4 that is a given distance along this Ray
   * @param t - the distance along the Ray to retrieve a position for.
   */
  Vec4 at(const float& t);

  /** @return Distance from the Vec4 point to the origin */
  float distanceToPoint(const Vec4& point);

  /**
   * @param minCorner - pointer to min corner position. (bottom left)
   * @param maxCorner - pointer to max corner position. (top right)
   * @param distance - the box to intersect with.
   * @return Vec4 point of intersection
   */
  u8 intersectBox(const Vec4& minCorner, const Vec4& maxCorner,
                  float& distance);

  /** Returns inverse direction */
  const Vec4 invDir();

 private:
  Vec4 _origin;
  Vec4 _direction;
};

}  // namespace Tyra