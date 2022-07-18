/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020-2022, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Wellinator Carvalho <wellcoj@gmail.com>
*/

#include "physics/ray.hpp"
#include <tamtypes.h>
#include <math.h>
#include <algorithm>

namespace Tyra {

Ray::Ray() {}
Ray::Ray(Vec4* origin, Vec4* direction) {
  this->origin.set(origin->x, origin->y, origin->z);
  this->direction.set(direction->x, direction->y, direction->z);
}

Ray::~Ray() {}

void Ray::set(Vec4 origin, Vec4 direction) {
  this->origin.set(origin);
  this->direction.set(direction);
}

void Ray::setOrigin(Vec4 origin) { this->origin.set(origin); }

void Ray::setOrigin(const float& t_x, const float& t_y, const float& t_z) {
  this->origin.set(t_x, t_y, t_z);
}

void Ray::setDirection(Vec4 direction) { this->direction.set(direction); }

void Ray::setDirection(const float& t_x, const float& t_y, const float& t_z) {
  this->direction.set(t_x, t_y, t_z);
}

float Ray::distanceToPoint(Vec4 point) { return origin.distanceTo(point); }

Vec4 Ray::at(float t) { return (this->direction * t) + this->origin; }

u8 Ray::intersectBox(Vec4* minCorner, Vec4* maxCorner, float& distance) {
  float tmin, tmax, tymin, tymax, tzmin, tzmax;
  Vec4 invDir = this->invDir();
  invDir.normalize();

  tmin = (minCorner->x - this->origin.x) * invDir.x;
  tmax = (maxCorner->x - this->origin.x) * invDir.x;
  tymin = (minCorner->y - this->origin.y) * invDir.y;
  tymax = (maxCorner->y - this->origin.y) * invDir.y;

  if ((tmin > tymax) || (tymin > tmax)) {
    distance = -1.0f;
    return 0;
  }

  if (tymin > tmin) tmin = tymin;

  if (tymax < tmax) tmax = tymax;

  tzmin = (minCorner->z - this->origin.z) * invDir.z;
  tzmax = (maxCorner->z - this->origin.z) * invDir.z;

  if ((tmin > tzmax) || (tzmin > tmax)) {
    distance = -1.0f;
    return 0;
  }

  if (tzmin > tmin) tmin = tzmin;

  if (tzmax < tmax) tmax = tzmax;

  if (tmax < 0) {
    distance = -1.0f;
    return 0;
  }

  distance = tmin >= 0 ? tmin : tmax;
  return 1;
}

Vec4 Ray::invDir() {
  return Vec4(1 / this->direction.x, 1 / this->direction.y,
                 1 / this->direction.z);
}

}  // Namespace Tyra