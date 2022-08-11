/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022-2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Wellinator Carvalho <wellcoj@gmail.com>
*/

#include "physics/ray.hpp"
#include <tamtypes.h>
#include <math.h>
#include <algorithm>

namespace Tyra {

Ray::Ray() {}
Ray::Ray(const Vec4& origin, const Vec4& direction) {
  this->_origin.set(origin);
  this->_direction.set(direction);
}

Ray::~Ray() {}

void Ray::set(const Vec4& origin, const Vec4& direction) {
  this->_origin.set(origin);
  this->_direction.set(direction);
}

void Ray::setOrigin(const Vec4& origin) { this->_origin.set(origin); }

void Ray::setDirection(const Vec4& direction) {
  this->_direction.set(direction);
}

Vec4 Ray::at(const float& t) { return (this->_direction * t) + this->_origin; }

float Ray::distanceToPoint(const Vec4& point) {
  return this->_origin.distanceTo(point);
}

u8 Ray::intersectBox(const Vec4& minCorner, const Vec4& maxCorner,
                     float& distance) {
  float tmin, tmax, tymin, tymax, tzmin, tzmax;
  Vec4 invDir = this->invDir();
  invDir.normalize();

  tmin = (minCorner.x - this->_origin.x) * invDir.x;
  tmax = (maxCorner.x - this->_origin.x) * invDir.x;
  tymin = (minCorner.y - this->_origin.y) * invDir.y;
  tymax = (maxCorner.y - this->_origin.y) * invDir.y;

  if ((tmin > tymax) || (tymin > tmax)) {
    distance = -1.0f;
    return 0;
  }

  if (tymin > tmin) tmin = tymin;

  if (tymax < tmax) tmax = tymax;

  tzmin = (minCorner.z - this->_origin.z) * invDir.z;
  tzmax = (maxCorner.z - this->_origin.z) * invDir.z;

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

const Vec4 Ray::invDir() {
  return Vec4(1 / this->_direction.x, 1 / this->_direction.y,
              1 / this->_direction.z, 1);
}

}  // Namespace Tyra