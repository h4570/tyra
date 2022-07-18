/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020-2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
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
  /**
   * Constructor create a ampty ray
   *
   */
  Ray();

  /**
   * Constructor
   * @param origin The origin of the ray
   * @param direction The direction of the ray (normalized Vec4)
   *
   */
  Ray(Vec4* origin, Vec4* direction);
  ~Ray();

  Vec4 origin;
  Vec4 direction;

  // Methods

  /**
   * Set origin and direction
   */
  void set(Vec4 origin, Vec4 direction);

  /**
   * Set origin only
   */
  void setOrigin(Vec4 origin);
  void setOrigin(const float& t_x, const float& t_y, const float& t_z);

  /**
   * Set direction only
   */
  void setDirection(Vec4 origin);
  void setDirection(const float& t_x, const float& t_y, const float& t_z);

  /**
   * Return Vec4 that is a given distance along this Ray
   * @param t - the distance along the Ray to retrieve a position for.
   */
  Vec4 at(float t);

  /**
   * Return the distance from the Vec4 point to the origin
   */
  float distanceToPoint(Vec4 point);

  /**
   * Returns distance from origin to intersected mesh
   */
  float distanceTo(const Vec4& v) const;

  /**
   * Returns Vec4 of intersection position
   * @param minCorner - pointer to min corner position. (bottom left)
   * @param maxCorner - pointer to max corner position. (top right)
   * @param distance - the box to intersect with.
   *
   */
  u8 intersectBox(Vec4* minCorner, Vec4* maxCorner, float& distance);

  /**
   * Returns distance from origin to intersected mesh
   */
  inline const Vec4 getPosition() { return _position; };

  /**
   * Returns inverse direction
   */
  Vec4 invDir();

 private:
  Vec4 _position;
};

}  // namespace Tyra