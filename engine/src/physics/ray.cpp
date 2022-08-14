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
#include <sstream>
#include <iomanip>

namespace Tyra {

Ray::Ray() {}
Ray::Ray(const Vec4& t_origin, const Vec4& t_direction) {
  origin.set(t_origin);
  direction.set(t_direction);
}

Ray::~Ray() {}

Vec4 Ray::at(const float& t) const { return (direction * t) + origin; }

float Ray::distanceToPoint(const Vec4& point) const {
  return origin.distanceTo(point);
}

bool Ray::intersectBox(const Vec4& minCorner, const Vec4& maxCorner,
                       float* outputDistance) const {
  auto inv = invDir();
  inv.normalize();

  float tmin = (minCorner.x - this->origin.x) * inv.x;
  float tmax = (maxCorner.x - this->origin.x) * inv.x;
  float tymin = (minCorner.y - this->origin.y) * inv.y;
  float tymax = (maxCorner.y - this->origin.y) * inv.y;

  if ((tmin > tymax) || (tymin > tmax)) {
    return false;
  }

  if (tymin > tmin) tmin = tymin;

  if (tymax < tmax) tmax = tymax;

  float tzmin = (minCorner.z - this->origin.z) * inv.z;
  float tzmax = (maxCorner.z - this->origin.z) * inv.z;

  if ((tmin > tzmax) || (tzmin > tmax)) {
    return false;
  }

  if (tzmin > tmin) tmin = tzmin;

  if (tzmax < tmax) tmax = tzmax;

  if (tmax < 0) {
    return false;
  }

  if (outputDistance != nullptr) {
    *outputDistance = tmin >= 0 ? tmin : tmax;
  }

  return true;
}

Vec4 Ray::invDir() const {
  return Vec4(1 / this->direction.x, 1 / this->direction.y,
              1 / this->direction.z, 1);
}

void Ray::print() const {
  auto text = getPrint(nullptr);
  printf("%s\n", text.c_str());
}

void Ray::print(const char* name) const {
  auto text = getPrint(name);
  printf("%s\n", text.c_str());
}

std::string Ray::getPrint(const char* name) const {
  std::stringstream res;
  if (name) {
    res << name << "(";
  } else {
    res << "Ray(";
  }
  res << std::fixed << std::setprecision(4);
  res << origin.getPrint("origin") << ", " << direction.getPrint("direction")
      << ")";
  return res.str();
}

}  // Namespace Tyra